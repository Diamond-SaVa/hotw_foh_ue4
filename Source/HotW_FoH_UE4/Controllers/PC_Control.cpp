// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "PC_Control.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "HotW_FoH_UE4/Characters/PC_C_Prince.h"
#include "HotW_FoH_UE4/Interfaces/Manager.h"
#include "HotW_FoH_UE4/UI/UW_CharacterSwitch.h"


// Sets default values
APC_Control::APC_Control()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PCharacterArray.Reserve(2);
}

// Called when the game starts or when spawned
void APC_Control::BeginPlay()
{
	Super::BeginPlay();
	
	if (MainWidgetClass != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("MainWidgetClass VALID!"));
		
		UUserWidget* UserWidget = CreateWidget<UUserWidget>(this, MainWidgetClass);
		
		MainWidgetInstance = Cast<UUW_CharacterSwitch>(UserWidget);
		
		if (MainWidgetInstance != nullptr)
		{
			MainWidgetInstance->AddToPlayerScreen();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("MainWidgetInstance INVALID?!"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("MainWidgetClass INVALID!?"));
	}
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
		if (IA_Move_Left != nullptr && IA_Move_Right != nullptr)
		{
			// Walking and Run
			EIC->BindAction(IA_Move_Left, 
				ETriggerEvent::Triggered,
				this,
				&APC_Control::ActionMovementLeft);
			
			EIC->BindAction(IA_Move_Right, 
				ETriggerEvent::Triggered,
				this, 
				&APC_Control::ActionMovementRight);
			
			EIC->BindAction(IA_Move_Left, 
				ETriggerEvent::Completed, 
				this, 
				&APC_Control::ActionMovementLeftStop);
			
			EIC->BindAction(IA_Move_Right, 
				ETriggerEvent::Completed,
				this, 
				&APC_Control::ActionMovementRightStop);
			
			// Stop Wall Sliding conditions 
			// EIC->BindAction(IA_Move, ETriggerEvent::Completed, this, &APC_Control::ActionStopWallSliding);
		}
		
		/* Jumping Actions (Bottom Face Button) */
		if (IA_Jump != nullptr)
		{
			// Ledge Jump Action
			EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &APC_Control::ActionJumpStart);
			EIC->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &APC_Control::ActionJumpHold);
			EIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &APC_Control::ActionJumpEnd);
		}
		
		/* Attacking Actions (Left Face Button) */
		if (IA_Attack != nullptr)
		{
			EIC->BindAction(IA_Attack, ETriggerEvent::Started, this, &APC_Control::ActionAttack);
		}
		
		/* Interaction Button (Right Face Button) */
		if (IA_Interact != nullptr)
		{
			// Interact
			EIC->BindAction(IA_Interact, ETriggerEvent::Started, this, &APC_Control::ActionInteractStart);
		}
		
		/* Modifier Actions (D-Pad/Left Joystick Up and Down) */
		if (IA_LookUp != nullptr)
		{
			// Look Up
			EIC->BindAction(IA_LookUp, ETriggerEvent::Started, this, &APC_Control::ActionPressUpStart);
			EIC->BindAction(IA_LookUp, ETriggerEvent::Completed, this, &APC_Control::ActionPressUpEnd);
		}
		
		if (IA_LookDown != nullptr)
		{
			// Crouch
			EIC->BindAction(IA_LookDown, ETriggerEvent::Started, this, &APC_Control::ActionPressDownStart);
			EIC->BindAction(IA_LookDown, ETriggerEvent::Completed, this, &APC_Control::ActionPressDownEnd);
		}
		
		/* Dodge and Dash Abilities (Left Shoulder Button) */
		if (IA_Dodge != nullptr)
		{
			// Dodge
			EIC->BindAction(IA_Dodge, ETriggerEvent::Started, this, &APC_Control::ActionDodgeStart);
		}
		
		if (IA_Switch != nullptr)
		{
			// Switch
			EIC->BindAction(IA_Switch, ETriggerEvent::Started, this, &APC_Control::ActionBufferSwitch);
		}
	}
	/**/
}

void APC_Control::CharacterSetUpFromBeginPlay(APC_C_Prince* PlayableCharacter, const int32 CharacterID)
{
	if (IsValid(PlayableCharacter) == false)
	{
		return;
	}
	
	PCharacterArray[CharacterID] = PlayableCharacter;
	
	if (CharacterID != CurrentCharacterSlot)
	{
		PCharacterArray[CharacterID]->DeactivateCharacter();
	}
	else
	{
		PCharacterArray[CharacterID]->ActivateCharacter(this);
		
		SendCharacterToManager();
	}
}

void APC_Control::ReenableSwitch()
{
	CanSwitchCharacter = true;
}

void APC_Control::ActionAttack()
{
	if (PCharacterArray[CurrentCharacterSlot].IsValid() == false)
	{
		return;
	}
	
	PCharacterArray[CurrentCharacterSlot].Get()->Attack();
}

void APC_Control::ActionMovement(const FInputActionValue& Value)
{
	if (PCharacterArray[CurrentCharacterSlot].IsValid() == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, "FAILED AT CAST");
		return;
	}
	
	if (MovingLeft == true && MovingRight == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, "FAILED AT CAST");
		
		ActionMovementStop();
		
		return;
	}
	
	PCharacterArray[CurrentCharacterSlot].Get()->Move(Value.Get<float>());
}

void APC_Control::ActionMovementLeft(const FInputActionValue& Value)
{
	MovingLeft = true;
	
	ActionMovement(Value);
}

void APC_Control::ActionMovementRight(const FInputActionValue& Value)
{
	MovingRight = true;
	
	ActionMovement(Value);
}

void APC_Control::ActionMovementStop()
{
	if (PCharacterArray[CurrentCharacterSlot].IsValid() == false)
	{
		return;
	}
	
	PCharacterArray[CurrentCharacterSlot].Get()->MoveEnd();
}

void APC_Control::ActionMovementLeftStop()
{
	MovingLeft = false;
	
	ActionMovementStop();
}

void APC_Control::ActionMovementRightStop()
{
	MovingRight = false;
	
	ActionMovementStop();
}

void APC_Control::ActionJumpStart()
{
	if (PCharacterArray[CurrentCharacterSlot].IsValid() == false)
	{
		return;
	}
	
	PCharacterArray[CurrentCharacterSlot].Get()->JumpStart();
}

void APC_Control::ActionJumpHold()
{
	if (PCharacterArray[CurrentCharacterSlot].IsValid() == false)
	{
		return;
	}
	
	PCharacterArray[CurrentCharacterSlot].Get()->JumpHold();
}

void APC_Control::ActionJumpEnd()
{
	if (PCharacterArray[CurrentCharacterSlot].IsValid() == false)
	{
		return;
	}
	
	PCharacterArray[CurrentCharacterSlot].Get()->JumpEnd();
}

void APC_Control::ActionStopWallSliding()
{
	if (PCharacterArray[CurrentCharacterSlot].IsValid() == false)
	{
		return;
	}
	
	if (PCharacterArray[CurrentCharacterSlot].Get()->GetMovementState() == EMovementState::ECS_WallState)
	{
		PCharacterArray[CurrentCharacterSlot].Get()->SetMovementState(EMovementState::ECS_AirState);
	}
	
	PCharacterArray[CurrentCharacterSlot].Get()->ResetWallLedgeComponentDetection();
}

void APC_Control::ActionInteractStart()
{
	if (PCharacterArray[CurrentCharacterSlot].IsValid() == false)
	{
		return;
	}
	
	PCharacterArray[CurrentCharacterSlot].Get()->Interact();
}

void APC_Control::ActionPressUpStart()
{
	if (PCharacterArray[CurrentCharacterSlot].IsValid() == false)
	{
		return;
	}
	
	PCharacterArray[CurrentCharacterSlot].Get()->SetIsLookingUp(true);
	
	PCharacterArray[CurrentCharacterSlot].Get()->LedgeUpAnim();
	
	PCharacterArray[CurrentCharacterSlot].Get()->SetIsLookingDown(false);
	
	PCharacterArray[CurrentCharacterSlot].Get()->UnCrouch();
}

void APC_Control::ActionPressUpEnd()
{
	if (PCharacterArray[CurrentCharacterSlot].IsValid() == false)
	{
		return;
	}
	
	PCharacterArray[CurrentCharacterSlot].Get()->SetIsLookingUp(false);
}

void APC_Control::ActionPressDownStart()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, "DOWN CALLED");
	
	if (PCharacterArray[CurrentCharacterSlot].IsValid() == false)
	{
		return;
	}
	
	PCharacterArray[CurrentCharacterSlot].Get()->SetIsLookingDown(true);
	
	if (PCharacterArray[CurrentCharacterSlot].Get()->GetMovementState() == EMovementState::ECS_LedgeState && 
		PCharacterArray[CurrentCharacterSlot].Get()->GetActionState() == EActionState::EAct_NilState)
	{
		PCharacterArray[CurrentCharacterSlot].Get()->LedgeDownAnim();
	}
	
	PCharacterArray[CurrentCharacterSlot].Get()->CrouchStart();
	
	PCharacterArray[CurrentCharacterSlot].Get()->SetIsLookingUp(false);
}

void APC_Control::ActionPressDownEnd()
{
	if (PCharacterArray[CurrentCharacterSlot].IsValid() == false)
	{
		return;
	}
	
	PCharacterArray[CurrentCharacterSlot].Get()->SetIsLookingDown(false);
	
	PCharacterArray[CurrentCharacterSlot].Get()->CrouchEnd();
}

void APC_Control::ActionDodgeStart()
{
	if (PCharacterArray[CurrentCharacterSlot].IsValid() == false)
	{
		return;
	}
	
	PCharacterArray[CurrentCharacterSlot].Get()->DodgeFromAnimation();
}

void APC_Control::ActionBufferSwitch()
{
	if (CanSwitchCharacter == false ||
		PCharacterArray[CurrentCharacterSlot].Get()->GetActionState() != EActionState::EAct_NilState)
	{
		return;
	}
	
	if (NextCharacterSlot == 0)
	{
		NextCharacterSlot = 1;
	}
	else if (NextCharacterSlot == 1)
	{
		NextCharacterSlot = 0;
	}
	
	MainWidgetInstance->SwitchToCharacter(NextCharacterSlot);
	
	CanSwitchCharacter = false;
	
	SwitchCharacterAfterBuffer();
	
	ActivateSwitchBlockTimer();
}

void APC_Control::ActionSwitchCharacter()
{
	if (PCharacterArray[CurrentCharacterSlot].IsValid() == false || 
		PCharacterArray[CurrentCharacterSlot].Get()->GetMovementState() != EMovementState::ECS_GroundState || 
		PCharacterArray[CurrentCharacterSlot].Get()->GetActionState() != EActionState::EAct_NilState ||
		NextCharacterSlot == CurrentCharacterSlot)
	{
		return;
	}
	
	SwitchCharacterProcess();
	
	ActivateSwitchBlockTimer();
}

void APC_Control::SwitchCharacterAfterBuffer()
{
	if (PCharacterArray[CurrentCharacterSlot].IsValid() == false || 
		PCharacterArray[CurrentCharacterSlot].Get()->GetMovementState() != EMovementState::ECS_GroundState || 
		NextCharacterSlot == CurrentCharacterSlot)
	{
		return;
	}
	
	SwitchCharacterProcess();
	
	ActivateSwitchBlockTimer();
}


void APC_Control::SwitchCharacterFromLanding()
{
	if (NextCharacterSlot == CurrentCharacterSlot)
	{
		return;
	}
	
	SwitchCharacterProcess();
	
	ActivateSwitchBlockTimer();
}

void APC_Control::SwitchCharacterProcess()
{
	const FVector Location = PCharacterArray[CurrentCharacterSlot].Get()->GetActorLocation();
	
	const FRotator Rotation = PCharacterArray[CurrentCharacterSlot].Get()->GetActorRotation();
	
	PCharacterArray[CurrentCharacterSlot].Get()->DeactivateCharacter();
	
	CurrentCharacterSlot = NextCharacterSlot;
	
	PCharacterArray[CurrentCharacterSlot].Get()->SetActorLocation(Location);
	
	PCharacterArray[CurrentCharacterSlot].Get()->SetActorRotation(Rotation);
	
	PCharacterArray[CurrentCharacterSlot].Get()->ActivateCharacter(this);
	
	SendCharacterToManager();
	
	CanSwitchCharacter = false;
}

void APC_Control::ActivateSwitchBlockTimer()
{
	FTimerManager& WorldTimer = GetWorldTimerManager();
	
	// In the case that the Timer is Active and the Character lands, ready to Switch, deactivate it so the 
	// switching button is not available to be pressed until the switch is done.
	if (WorldTimer.IsTimerActive(ReenableSwitchTimerHandle) == true)
	{
		WorldTimer.ClearTimer(ReenableSwitchTimerHandle);
	}
	
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
		Manager->SetActiveCharacter(CurrentCharacterSlot, PCharacterArray[CurrentCharacterSlot].Get());
	}
}

void APC_Control::RegisterCharacterToControllerArray(APC_C_Prince* PlayableCharacter, const int32 CharacterID)
{
	CharacterSetUpFromBeginPlay(PlayableCharacter, CharacterID);
}

void APC_Control::CheckIfCouldSwitchOnLanded()
{
	SwitchCharacterFromLanding();
}
