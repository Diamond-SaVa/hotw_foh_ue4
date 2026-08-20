// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "PC_A_Base.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "HotW_FoH_UE4/AnimClasses/AnimBPs/AnimPrince.h"

// Sets default values
APC_A_Base::APC_A_Base()
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	constexpr ECollisionChannel Channel = ECC_Camera;
	constexpr ECollisionResponse Response = ECR_Ignore;
	
	UCapsuleComponent* CapComp = GetCapsuleComponent();
	if (CapComp != nullptr)
	{
		CapComp->SetCollisionResponseToChannel(Channel, Response);
	}
	
	USkeletalMeshComponent* SMComp = GetMesh();
	if (SMComp != nullptr)
	{
		SMComp->SetCollisionResponseToChannel(Channel, Response);
	}
}

// Called when the game starts or when spawned
void APC_A_Base::BeginPlay()
{
	Super::BeginPlay();
	
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	
	if (MoveComp != nullptr)
	{
		MoveComp->AirControl = 1.0f;
		MoveComp->MaxWalkSpeed = MovementWalkMinSpeed;
		MoveComp->MaxWalkSpeedCrouched = 0.0f;
		MoveComp->MaxSwimSpeed = MovementSwimMinSpeed;
		MoveComp->GravityScale = GravityDefault;
	}
}

// Called to bind functionality to input
void APC_A_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APC_A_Base::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	const EMovementMode NewMovementMode = GetCharacterMovement()->MovementMode;
	
	switch (NewMovementMode)
	{
	case MOVE_Walking:
		SetMovementState(EMovementState::ECS_GroundState);
		break;;
	case MOVE_Falling:
		SetMovementState(EMovementState::ECS_AirState);
		break;
	case MOVE_Swimming:
		SetMovementState(EMovementState::ECS_SwimState);
		break;
	default:
		break;
	}
}

void APC_A_Base::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();
	
	// Plays the Animation found within the Anim Montage and from a specific Section
	PlayAnimMontage_Safe(AM_Jump);

	SetBoolGoingUp(true);
	
	// Resets the bNotifyApex to true for each jump
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->bNotifyApex = true;
	
	SetActionState(EActionState::EAct_NilState);
}

void APC_A_Base::NotifyJumpApex()
{
	Super::NotifyJumpApex();
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("APEX"));
	
	UAnimInstance* AnimBP = GetMesh()->GetAnimInstance();
	if (IsValid(AnimBP))
	{
		if (AnimBP->Montage_IsPlaying(AM_Jump))
		{
			AnimBP->Montage_Stop(AM_Jump->BlendOut.GetBlendTime(), AM_Jump);
		}
	}
	
	SetBoolGoingUp(false);
	
	if (ActionState != EActionState::EAct_NilState && ActionState != EActionState::EAct_PerfectDodgeState &&
		ActionState != EActionState::EAct_DodgeState)
	{
		SetActionState(EActionState::EAct_NilState);
	}
}

void APC_A_Base::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	FName SlotName = FName("Move");
	
	if (StickDirection == 0.0f)
	{
		SlotName = FName("Default");
		
		UCharacterMovementComponent* MoveComp = GetCharacterMovement();
		if (IsValid(MoveComp) == true)
		{
			MoveComp->StopMovementImmediately();
		}
	}
	
	PlayAnimMontage_Safe(AM_Landing, SlotName);
}

void APC_A_Base::SetMovementState(const EMovementState NewState)
{
	MovementState = NewState;

	UAnimInstance* AnimBP = GetMesh()->GetAnimInstance();
	
	if (AnimBP == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("ANIMBP NOT FOUND"));
		return;
	}
	
	if (IStateSetters* SetterInterface = Cast<IStateSetters>(AnimBP))
	{
		SetterInterface->SetMovementState(NewState);
	}
}

void APC_A_Base::SetActionState(const EActionState NewState)
{
	ActionState = NewState;
	
	UAnimInstance* AnimBP = GetMesh()->GetAnimInstance();
	
	if (AnimBP == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("ANIMBP NOT FOUND"));
		return;
	}
	
	if (IStateSetters* SetterInterface = Cast<IStateSetters>(AnimBP))
	{
		SetterInterface->SetActionState(NewState);
	}
}

void APC_A_Base::SetSpeedState(const ESpeedState NewState)
{
	SpeedState = NewState;
	
	UAnimInstance* AnimBP = GetMesh()->GetAnimInstance();
	
	if (AnimBP == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("ANIMBP NOT FOUND"));
		return;
	}
	
	if (IStateSetters* SetterInterface = Cast<IStateSetters>(AnimBP))
	{
		SetterInterface->SetSpeedState(NewState);
	}
}

void APC_A_Base::SetBoolGoingUp(const bool IsGoingUp)
{
	UAnimInstance* AnimBP = GetMesh()->GetAnimInstance();
	
	if (AnimBP == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("ANIMBP NOT FOUND"));
		return;
	}
	
	if (IStateSetters* SetterInterface = Cast<IStateSetters>(AnimBP))
	{
		SetterInterface->SetBoolGoingUp(IsGoingUp);
	}
}

void APC_A_Base::Move(float Value)
{
	// Captures the Value and rounds it to -1.0f or 1.0f
	StickDirection = FMath::RoundToFloat(Value);
	
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
	Value = FMath::Abs(Value);
	
	if (ActionState == EActionState::EAct_SideAttackState)
	{
		Value *= 0.5f;
	}
		
	// Gets the pointer to the CharacterMovementComponent
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	
	// Safe check to guarantee changes for the Character Movement 
	if (MoveComp != nullptr)
	{
		// Keep the stick under half-way, and make the character move at walk speed.
		if (Value <= 0.75f)
		{
			if (MoveComp->MaxWalkSpeed != MovementWalkMinSpeed)
			{
				MoveComp->MaxWalkSpeed = MovementWalkMinSpeed;
			}
			
			if (SpeedState != ESpeedState::ESP_Walk)
			{
				SetSpeedState(ESpeedState::ESP_Walk);
			}
		} 
		// Push the stick farther, and now the character will run.
		else if (Value > 0.75f)
		{
			if (MoveComp->MaxWalkSpeed != MovementWalkMaxSpeed)
			{
				MoveComp->MaxWalkSpeed = MovementWalkMaxSpeed;
			}
			
			if (SpeedState != ESpeedState::ESP_Run)
			{
				SetSpeedState(ESpeedState::ESP_Run);
			}
		}
	}

	// Sets the movement direction directly to 1.0f or -1.0f with the previous configuration of MaxWalkSpeed set
	// according to how far the stick is pushed on any direction
	const FVector MovementDirection = FVector(StickDirection, 0.0, 0.0);
	
	// Uses the obtained StickDirection variable to apply movement.
	AddMovementInput(MovementDirection);
}

void APC_A_Base::MoveEnd()
{	
	SetSpeedState(ESpeedState::ESP_Stop);
	
	// Resets stick direction to zero
	StickDirection = 0.0f;
}

void APC_A_Base::JumpStart()
{
	if (MovementState == EMovementState::ECS_CrouchState)
	{
		UnCrouch();
	}
	
	// Checks if the character is on the ground, crouching or in the air
	if (ActionState == EActionState::EAct_NilState || ActionState == EActionState::EAct_DodgeState)
	{
		Jump();
	}
}

void APC_A_Base::JumpEnd()
{
	StopJumping();
}

void APC_A_Base::PlayAnimMontage_Safe(UAnimMontage* AnimMontage, const FName SectionName, float InPlayRate)
{
	// Also checks if the Skeletal Mesh Component still exists
	const USkeletalMeshComponent* SMesh = GetMesh();
	if (SMesh == nullptr || SMesh->GetAnimInstance() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Skeletal Mesh of %s NOT FOUND. Stopping Attack."), *GetName());
		return;
	}
	
	PlayAnimMontage(AnimMontage, InPlayRate, SectionName);
}

void APC_A_Base::LineTraceResponse(const FVector& Direction, const float Distance, FHitResult& Hit, 
                                   const ECollisionChannel CollisionChannel, const FCollisionQueryParams& QueryParams, bool& bResult) const
{
	// Sets the start and end location according to the given direction
	const FVector StartLocation = GetActorLocation();
	const FVector EndLocation = GetActorLocation() + (Direction * Distance);
	
	// Returns if the trace managed to hit something
	bResult = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, CollisionChannel, QueryParams);
}

void APC_A_Base::SetTerminalVelocity(const float NewTerminal)
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (IsValid(MoveComp) == false)
	{
		return;
	}
	
	MoveComp->GetPhysicsVolume()->TerminalVelocity = NewTerminal;
}

void APC_A_Base::StopTerminalVelocity()
{
	
}

void APC_A_Base::PrintDebug(const FString& Message, const float PrintTime)
{
	GEngine->AddOnScreenDebugMessage(-1, PrintTime, FColor::Black, Message);
}
