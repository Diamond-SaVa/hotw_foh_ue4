// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "PC_Control.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HotW_FoH_UE4/Characters/PC_C_Prince.h"
#include "GameFramework/GameModeBase.h"
#include "HotW_FoH_UE4/Interfaces/Manager.h"


// Sets default values
APC_Control::APC_Control()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PCArray.Reserve(2);
}

// Called when the game starts or when spawned
void APC_Control::BeginPlay()
{
	Super::BeginPlay();
}

void APC_Control::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// Add Input Mapping Context via LocalPlayer Subsystem
	if (ULocalPlayer* SubsystemLocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = SubsystemLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext != nullptr)
			{
				InputSubsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
	/**/
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{		
		/* Movement Actions (D-Pad/Left Joystick) */
		// Walking and Run
		EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APC_Control::ActionMovement);
		EIC->BindAction(IA_Move, ETriggerEvent::Completed, this, &APC_Control::ActionMovementStop);
		
		/* Jumping Actions (Bottom Face Button) */
		// Ledge Jump Action
		EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &APC_Control::ActionJumpStart);
		EIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &APC_Control::ActionJumpEnd);
		
		/* Attacking Actions (Left Face Button) */
		EIC->BindAction(IA_Attack, ETriggerEvent::Started, this, &APC_Control::ActionAttack);
		
		/* Modifier Actions for the InputMovement for camera shifting */
		EIC->BindAction(IA_Move, ETriggerEvent::Completed, this, &APC_Control::ActionStopWallSliding);
			
		/* Modifier Actions (D-Pad/Left Joystick Up and Down) */
		// Interact
		EIC->BindAction(IA_Interact, ETriggerEvent::Started, this, &APC_Control::ActionInteractStart);
		
		// Look Up
		EIC->BindAction(IA_LookUp, ETriggerEvent::Started, this, &APC_Control::ActionPressUpStart);
		EIC->BindAction(IA_LookUp, ETriggerEvent::Completed, this, &APC_Control::ActionPressUpEnd);
		
		// Crouch
		EIC->BindAction(IA_Crouch, ETriggerEvent::Started, this, &APC_Control::ActionPressDownStart);
		EIC->BindAction(IA_Crouch, ETriggerEvent::Completed, this, &APC_Control::ActionPressDownEnd);
		
		// Dodge
		EIC->BindAction(IA_Dodge, ETriggerEvent::Started, this, &APC_Control::ActionDodgeStart);
		
		// Switch
		EIC->BindAction(IA_Switch, ETriggerEvent::Started, this, &APC_Control::ActionSwitchCharacter);
	}
	/**/
}

void APC_Control::CharacterSetUpFromBeginPlay(APC_C_Prince* PlayableCharacter, const int32 CharacterID)
{
	if (IsValid(PlayableCharacter) == false)
	{
		return;
	}
	
	PCArray[CharacterID] = PlayableCharacter;
	
	if (CharacterID != CurrentCharacterSlot)
	{
		PCArray[CharacterID]->DeactivateCharacter();
	}
	else
	{
		ThePC = PCArray[CharacterID];
		
		ThePC->ActivateCharacter();
		
		SendCharacterToManager();
	}
}

void APC_Control::ReenableSwitch()
{
	CanSwitchCharacter = 0;
}

void APC_Control::ActionAttack()
{
	if (ThePC.IsValid() == false)
	{
		return;
	}
	
	ThePC.Get()->Attack();
}

void APC_Control::ActionMovement(const FInputActionValue& Value)
{
	if (ThePC.IsValid() == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, "FAILED AT CAST");
		return;
	}
	
	ThePC.Get()->Move(Value.Get<float>());
}

void APC_Control::ActionMovementStop()
{
	if (ThePC.IsValid() == false)
	{
		return;
	}
	
	ThePC.Get()->MoveEnd();
}

void APC_Control::ActionJumpStart()
{
	if (ThePC.IsValid() == false)
	{
		return;
	}
	
	ThePC.Get()->JumpStart();
}

void APC_Control::ActionJumpEnd()
{
	if (ThePC.IsValid() == false)
	{
		return;
	}
	
	ThePC.Get()->JumpEnd();
}

void APC_Control::ActionStopWallSliding()
{
	if (ThePC.IsValid() == false)
	{
		return;
	}
	
	if (ThePC.Get()->GetMovementState() == EMovementState::ECS_WallState)
	{
		ThePC.Get()->SetMovementState(EMovementState::ECS_AirState);
	}
	
	ThePC.Get()->SetWallDetectTop(false);
}

void APC_Control::ActionInteractStart()
{
	if (ThePC.IsValid() == false)
	{
		return;
	}
	
	ThePC.Get()->Interact();
}

void APC_Control::ActionPressUpStart()
{
	if (ThePC.IsValid() == false)
	{
		return;
	}
	
	ThePC.Get()->SetIsLookingUp(true);
	
	ThePC.Get()->LedgeUpAnim();
	
	ThePC.Get()->SetIsLookingDown(false);
	
	ThePC.Get()->UnCrouch();
}

void APC_Control::ActionPressUpEnd()
{
	if (ThePC.IsValid() == false)
	{
		return;
	}
	
	ThePC.Get()->SetIsLookingUp(false);
}

void APC_Control::ActionPressDownStart()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, "DOWN CALLED");
	
	if (ThePC.IsValid() == false)
	{
		return;
	}
	
	ThePC.Get()->SetIsLookingDown(true);
	
	if (ThePC.Get()->GetMovementState() == EMovementState::ECS_LedgeState && 
		ThePC.Get()->GetActionState() == EActionState::EAct_NilState)
	{
		ThePC.Get()->LedgeDownAnim();
	}
	
	ThePC.Get()->CrouchStart();
	
	ThePC.Get()->SetIsLookingUp(false);
}

void APC_Control::ActionPressDownEnd()
{
	if (ThePC.IsValid() == false)
	{
		return;
	}
	
	ThePC.Get()->SetIsLookingDown(false);
	
	ThePC.Get()->CrouchEnd();
}

void APC_Control::ActionDodgeStart()
{
	if (ThePC.IsValid() == false)
	{
		return;
	}
	
	ThePC.Get()->DodgeFromAnimation();
}

void APC_Control::ActionSwitchCharacter()
{
	if (CanSwitchCharacter == 1 || ThePC.IsValid() == false	|| 
		ThePC.Get()->GetMovementState() != EMovementState::ECS_GroundState || 
		ThePC.Get()->GetActionState() != EActionState::EAct_NilState)
	{
		return;
	}
	
	const FVector Location = ThePC.Get()->GetActorLocation();
	
	const FRotator Rotation = ThePC.Get()->GetActorRotation();
	
	ThePC.Get()->DeactivateCharacter();
	
	if (CurrentCharacterSlot == 0)
	{
		CurrentCharacterSlot = 1;
	}
	else if (CurrentCharacterSlot == 1)
	{
		CurrentCharacterSlot = 0;
	}
	
	ThePC = PCArray[CurrentCharacterSlot];
	
	ThePC.Get()->SetActorLocation(Location);
	
	ThePC.Get()->SetActorRotation(Rotation);
	
	ThePC.Get()->ActivateCharacter();
	
	SendCharacterToManager();
	
	CanSwitchCharacter = 1;
	
	FTimerManager& WorldTimer = GetWorldTimerManager();
	if (WorldTimer.IsTimerActive(ReenableSwitchTimerHandle) == false)
	{
		WorldTimer.SetTimer(
		ReenableSwitchTimerHandle,			// The tracking handle
		this,								// The context object running the function
		&APC_Control::ReenableSwitch,		// The address of your function
		1.0f,								// Time in seconds between executions
		false								// True to loop continuously, False to run only once
		);
	}
}

void APC_Control::SendCharacterToManager()
{
	UWorld* TheWorld = GetWorld();
	if (IsValid(TheWorld) == false)
	{
		return;
	}
	
	AGameModeBase* GM = TheWorld->GetAuthGameMode();
	if (IManager* Manager = Cast<IManager>(GM))
	{
		Manager->SetActiveCharacter(CurrentCharacterSlot, ThePC.Get());
	}
}

bool APC_Control::CanAssignControls() const
{
	const bool bResult = IsValid(IA_Move) && IsValid(IA_Attack) && IsValid(IA_Attack) && IsValid(IA_Interact) &&
			IsValid(IA_LookUp) && IsValid(IA_Crouch) && IsValid(IA_Dodge);
		
	return bResult;
}
