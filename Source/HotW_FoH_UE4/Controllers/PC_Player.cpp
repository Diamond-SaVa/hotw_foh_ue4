// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "PC_Player.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "HotW_FoH_UE4/Characters/BaseCharacter.h"
#include "HotW_FoH_UE4/Interfaces/Movement.h"

void APC_Player::BeginPlay()
{
	Super::BeginPlay();
	
	if (UWorld* World = GetWorld())
	{
		UE_LOG(LogClass, Warning, TEXT("WORLD"));
		
		if (CharacterClassArray.Num() > 0)
		{			
			if (TSubclassOf<ABaseCharacter> CharacterClass = CharacterClassArray[0].LoadSynchronous())
			{
				UE_LOG(LogClass, Warning, TEXT("CHARACTER CLASS"));
				
				FActorSpawnParameters SpawnParams;
				
				SpawnParams.SpawnCollisionHandlingOverride =
					ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			
				SpawnParams.Owner = this;
			
				SpawnLocation = FVector(0.0f, 0.0f, 150.0f);
				const FRotator SpawnRotation = FRotator::ZeroRotator;

				if (AActor* SpawnCharacter = World->SpawnActor<AActor>(
					CharacterClass,
					SpawnLocation,
					SpawnRotation,
					SpawnParams))
				{
					PCharacterArray.Add(SpawnCharacter);
				
					UE_LOG(LogClass, Warning, TEXT("SPAWNED"));
				}
				else
				{
					UE_LOG(LogClass, Warning, TEXT("NO SPAWN"));
				}
			}
			else
			{
				UE_LOG(LogClass, Warning, TEXT("NO CHAR CLASS"));
			}
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("NO WORLD"));
		}
	}
}

void APC_Player::SetupInputComponent()
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
	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		/* Movement Actions (D-Pad/Left Joystick) */
		if (IA_Move_Left != nullptr && IA_Move_Right != nullptr)
		{
			// Walking and Run
			EIC->BindAction(IA_Move_Left, 
				ETriggerEvent::Started,
				this,
				&APC_Player::ActionMovementLeftStart);
			
			EIC->BindAction(IA_Move_Right, 
				ETriggerEvent::Started,
				this, 
				&APC_Player::ActionMovementRightStart);
			
			EIC->BindAction(IA_Move_Left, 
				ETriggerEvent::Triggered,
				this,
				&APC_Player::ActionMovementHold);
			
			EIC->BindAction(IA_Move_Right, 
				ETriggerEvent::Triggered,
				this, 
				&APC_Player::ActionMovementHold);
			
			EIC->BindAction(IA_Move_Left, 
				ETriggerEvent::Triggered,
				this,
				&APC_Player::ActionMovementHold);
			
			EIC->BindAction(IA_Move_Right, 
				ETriggerEvent::Triggered,
				this, 
				&APC_Player::ActionMovementHold);
			
			EIC->BindAction(IA_Move_Left, 
				ETriggerEvent::Completed, 
				this, 
				&APC_Player::ActionMovementLeftStop);
			
			EIC->BindAction(IA_Move_Right, 
				ETriggerEvent::Completed,
				this, 
				&APC_Player::ActionMovementRightStop);
			
			// Stop Wall Sliding conditions 
			// EIC->BindAction(IA_Move, ETriggerEvent::Completed, this, &APC_Control::ActionStopWallSliding);
		}
	} 
}

void APC_Player::ActionMovementLeftStart()
{
	MovingLeft = true;
}

void APC_Player::ActionMovementLeftStop()
{
	MovingLeft = false;
}

void APC_Player::ActionMovementHold(const FInputActionValue& Value)
{
	if (PCharacterArray.Num() == 0 || (MovingLeft && MovingRight))
	{
		return;
	}
	
	if (AActor* ThisCharacter = PCharacterArray[CurrentCharacterIndex].Get())
	{
		if (IMovement* CharMove = Cast<IMovement>(ThisCharacter))
		{
			const float StickDirection = Value.Get<float>();
				
			// Call interface function move
			CharMove->MoveInputInterface(StickDirection);
		}
	}
}

void APC_Player::ActionMovementRightStart()
{
	MovingRight = true;
}

void APC_Player::ActionMovementRightStop()
{
	MovingRight = false;
}

void APC_Player::CheckIfCouldSwitchOnLanded()
{
	
}

void APC_Player::RegisterCharacterToControllerArray(AActor* PlayableCharacter, const int32 CharacterID)
{
	
}
