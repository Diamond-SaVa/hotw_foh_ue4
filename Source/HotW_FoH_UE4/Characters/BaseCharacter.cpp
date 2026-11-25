// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "BaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->AirControl = 1.0f;
		MoveComp->MaxWalkSpeed = MovementWalkMinSpeed;
		MoveComp->MaxWalkSpeedCrouched = 0.0f;
		
		/* GIRL exclusive setting * /
		MoveComp->MaxSwimSpeed = MovementSwimMinSpeed;
		/**/
		
		MoveComp->GravityScale = 1.5f;
		
		MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
		MoveComp->bOrientRotationToMovement = true;
		MoveComp->bUseControllerDesiredRotation = false;
		MoveComp->RotationRate = FRotator(0.0f, -1.0f, 0.0f);
	}
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();
}

void ABaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}

void ABaseCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
}

void ABaseCharacter::InteractStart()
{
}

void ABaseCharacter::MoveStart(float Direction)
{
}

void ABaseCharacter::MoveHold(float Direction)
{	
	// If the ActionState is anything but neutral, then do not execute further code
	if (ActionState != EActionState::EAct_NilState || MovementState == EMovementState::ECS_WallState ||
		MovementState == EMovementState::ECS_LedgeState)
	{
		if (ActionState != EActionState::EAct_SideAttackState)
		{
			return;
		}
	}
	
	// Stick Length according to how much the stick is being pushed by the player
	const float AbsStickRange = FMath::Abs(Direction);
		
	// Keep the stick under half-way, and make the character move at walk speed.
	if (AbsStickRange <= 0.75f)
	{
		if (SpeedState != ESpeedState::ESP_Walk)
		{
			SetSpeedState(ESpeedState::ESP_Walk);
		}
	} 
	// Push the stick farther, and now the character will run.
	else if (AbsStickRange > 0.75f)
	{
						
		if (SpeedState != ESpeedState::ESP_Run)
		{
			SetSpeedState(ESpeedState::ESP_Run);
		}
	}
	
	Direction = FMath::RoundToFloat(Direction);

	// Sets the movement direction directly to 1.0f or -1.0f with the previous configuration of MaxWalkSpeed set
	// according to how far the stick is pushed on any direction
	const FVector MovementDirection = FVector(Direction, 0.0, 0.0);
	
	// Uses the obtained StickDirection variable to apply movement.
	AddMovementInput(MovementDirection);
}

void ABaseCharacter::MoveStop()
{
}

void ABaseCharacter::JumpStart()
{
}

void ABaseCharacter::JumpHold()
{
}

void ABaseCharacter::JumpEnd()
{
}

void ABaseCharacter::AttackStart()
{
}

void ABaseCharacter::AttackHold()
{
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::DodgeStart()
{
}

void ABaseCharacter::DodgeHold()
{
}

void ABaseCharacter::DodgeEnd()
{
}

void ABaseCharacter::ActivateCharacter(AController* NewController)
{
}

void ABaseCharacter::DeactivateCharacter()
{
}

void ABaseCharacter::OnLedgeGrabStart()
{
}

void ABaseCharacter::OnLedgeGrabStop()
{
}

void ABaseCharacter::OnWallSlideStart()
{
}

void ABaseCharacter::OnWallSlideStop()
{
}

void ABaseCharacter::SetMovementState(const EMovementState NewState)
{
}

void ABaseCharacter::SetActionState(const EActionState NewState)
{
}

void ABaseCharacter::SetSpeedState(const ESpeedState NewState)
{
	SpeedState = NewState;
	
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		switch (NewState)
		{
		case ESpeedState::ESP_Walk:
			MoveComp->MaxWalkSpeed = MovementWalkMinSpeed;
			break;
			
		case ESpeedState::ESP_Run:
			MoveComp->MaxWalkSpeed = MovementWalkMaxSpeed;
			break;
			
		default:
			break;
		}
	}
}

void ABaseCharacter::SetMaskState(const EMaskState NewState)
{
}

void ABaseCharacter::SetBoolGoingUp(const bool IsGoingUp)
{
}

void ABaseCharacter::CharacterVelocityInterface(const FVector& NewVelocity)
{
}

void ABaseCharacter::CharacterVelocityInterface(const EDirection AxisDirection, const float NewVelocity)
{
}

void ABaseCharacter::DodgeInterface(const float DodgePower, const float DeltaSeconds, const EDirection DirectionEnum)
{
}

void ABaseCharacter::LedgeGrabStart()
{
}

void ABaseCharacter::LedgeJumpInterface()
{
}

void ABaseCharacter::MoveInputInterface(const float Direction)
{
	MoveHold(Direction);
}

void ABaseCharacter::WallSlideStart()
{
}

void ABaseCharacter::SphereTraceDamage(const FVector& StartLocation, const FVector& EndLocation,
	TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, float Radius)
{
}

void ABaseCharacter::TakeDamage(const int32 Damage)
{
}

void ABaseCharacter::KnockbackFromSource(const FVector& DamageSourceLocation, const float KnockbackPower)
{
}

