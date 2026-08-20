// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "PC_C_Prince.h"

#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "HotW_FoH_UE4/Components/AC_WallSlideComponent.h"
#include "HotW_FoH_UE4/Controllers/PC_Control.h"
#include "HotW_FoH_UE4/Interfaces/Interactions.h"


// Sets default values
APC_C_Prince::APC_C_Prince()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent != nullptr)
	{
		MovementComponent->GetNavAgentPropertiesRef().bCanCrouch = true;
		MovementComponent->bOrientRotationToMovement = true;
		MovementComponent->bUseControllerDesiredRotation = false;
		MovementComponent->RotationRate = FRotator(0.0f, -1.0f, 0.0f);
		
		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;
		bUseControllerRotationYaw = false;
	}
	
	WallSlideComponent = CreateDefaultSubobject<UAC_WallSlideComponent>(FName("WallSlideSkillComponent"));
}

// Called when the game starts or when spawned
void APC_C_Prince::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	if (IsValid(World) == true)
	{
		if (APC_Control* PC =  Cast<APC_Control>(World->GetFirstPlayerController()))
		{
			PC->CharacterSetUpFromBeginPlay(this, CharacterID);
		}
	}
	
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (IsValid(Capsule) == true)
	{
		HalfHeightMemory = Capsule->GetUnscaledCapsuleHalfHeight();
	}
	
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			// Bind the delegate to your custom function
			AnimInstance->OnMontageEnded.AddDynamic(this, &APC_C_Prince::HandleMontageEnded);
		}
	}
}

// Called to bind functionality to input
void APC_C_Prince::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APC_C_Prince::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	const EMovementMode NewMovementMode = GetCharacterMovement()->MovementMode;
	
	if (NewMovementMode == MOVE_Walking || NewMovementMode == MOVE_NavWalking)
	{
		CoyoteJumpCurrentCount = 0;
		PrintDebug("RESET COYOTE");
		
		if (ActionState == EActionState::EAct_AttackState || ActionState == EActionState::EAct_SideAttackState)
		{
			SetActionState(EActionState::EAct_NilState);
		}
		
		if (bIsLookingDown == true)
		{
			CrouchStart();
		}
	}
}

void APC_C_Prince::OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal,
	const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta)
{
	Super::OnWalkingOffLedge_Implementation(PreviousFloorImpactNormal, PreviousFloorContactNormal, 
		PreviousLocation, TimeDelta);
	
	PrintDebug("LedgeOff");
	
	FTimerManager& WorldTimer = GetWorldTimerManager();
	if (WorldTimer.IsTimerActive(CoyoteTimerHandle) == true)
	{
		return;
	}
	
	PrintDebug("Coyote Timer Start");
	
	WorldTimer.SetTimer(
		CoyoteTimerHandle,              // The tracking handle
		this,                     // The context object running the function
		&APC_C_Prince::CoyoteJumpDisable, // The address of your function
		0.3f,                     // Time in seconds between executions
		false                     // True to loop continuously, False to run only once
	);
}

void APC_C_Prince::Landed(const FHitResult& Hit)
{
	// Call parent Landed event so it can change the MovementSate to the Grounded State.
	Super::Landed(Hit);
	
	SetWallDetectTop(false);
	
	// If player is holding the button stick or D-Pad down, start crouching on landing
	if (bIsLookingDown == true)
	{
		CrouchStart();
	}
}

void APC_C_Prince::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	
	UAnimInstance* AnimBP = GetMesh()->GetAnimInstance();
	if (IsValid(AnimBP))
	{
		if (AnimBP->Montage_IsPlaying(AM_Landing))
		{
			AnimBP->Montage_Stop(AM_Landing->BlendOutTriggerTime, AM_Landing);	
		}
	}
	
	SetMovementState(EMovementState::ECS_CrouchState);
}

void APC_C_Prince::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	
	SetMovementState(EMovementState::ECS_GroundState);
}

void APC_C_Prince::SetMovementState(const EMovementState NewState)
{
	Super::SetMovementState(NewState);
	
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (IsValid(MoveComp) == false)
	{
		return;
	}
	
	switch (NewState)
	{
	case EMovementState::ECS_WallState:
		MoveComp->GetPhysicsVolume()->TerminalVelocity = 200.0f;
		CharacterVelocityInterface(FVector::ZeroVector);
		break;
	case EMovementState::ECS_LedgeState:
		MoveComp->SetMovementMode(MOVE_None);
		CharacterVelocityInterface(FVector::ZeroVector);
		break;
	default:
		// Return the terminal velocity to normal, as well as the 
		// component's movement mode if it was set to NONE before.
		MoveComp->GetPhysicsVolume()->TerminalVelocity = 4000.0f;
		if (MoveComp->MovementMode == MOVE_None)
		{
			MoveComp->SetMovementMode(MOVE_Falling);
		}
		break;
	}
}

void APC_C_Prince::SetActionState(const EActionState NewState)
{	
	Super::SetActionState(NewState);
	
	switch (NewState)
	{
	case EActionState::EAct_PerfectDodgeState:
	case EActionState::EAct_DodgeState:
		DashModifier = 0.0f;
		break;
	case EActionState::EAct_AttackState:
		if (MovementState == EMovementState::ECS_WallState)
		{
			SetTerminalVelocity(100.0f);
		}
		break;
	default:
		if (MovementState == EMovementState::ECS_WallState)
		{
			SetTerminalVelocity(200.0f);
		}
		else
		{
			SetTerminalVelocity(4000.0f);
		}
		
		if (bIsLookingDown == true)
		{
			CrouchStart();
		}
		else
		{
			CrouchEnd();
		}
		break;
	}
}

void APC_C_Prince::JumpStart()
{
	switch (MovementState)
	{
		case EMovementState::ECS_WallState:
		DodgeFromAnimation();
		return;
		
		case EMovementState::ECS_LedgeState:
		LedgeUpAnim();
		return;
		
		default:
		break;
	}
	
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (IsValid(Capsule) == false || IsValid(MoveComp) == false || CoyoteJumpCurrentCount == 1)
	{
		Super::JumpStart();
		return;
	}
	
	// Creates the boolean variable that checks if the character jumped from the ground or not 
	const bool OnGround = MoveComp->IsMovingOnGround();
		
	if (OnGround == true || CoyoteJumpCurrentCount == 1)
	{
		Super::JumpStart();
		return;
	}
	
	if (OnGround == false && CoyoteJumpCurrentCount == 0 && JumpCurrentCount < JumpMaxCount)
	{
		const float CoyoteJumpSpeed =  MoveComp->JumpZVelocity * 1.3f;
		LaunchCharacter(FVector::UpVector * CoyoteJumpSpeed, false, true);
		SetActionState(EActionState::EAct_NilState);
		CoyoteJumpCurrentCount = 1;
		PrintDebug("Coyote JUMPED");
		
		FTimerManager& WorldTimer = GetWorldTimerManager();
		if (WorldTimer.IsTimerActive(CoyoteTimerHandle))
		{
			WorldTimer.ClearTimer(CoyoteTimerHandle);
		}
		
		return;
	}
	
	Super::JumpStart();
}

void APC_C_Prince::HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (IsValid(Montage) == false)
	{
		return;
	}
	
	const bool bUnable = (ActionState == EActionState::EAct_HurtState || ActionState == EActionState::EAct_KOed_State);
	
	if (bInterrupted == true && bUnable == true)
	{
		
	}
}

void APC_C_Prince::DodgeInterface(const float DodgePower, const float DeltaSeconds, const EDirection DirectionEnum)
{
	if (IsValid(DodgeCurve) == false)
	{
		return;
	}
	
	// Prepares an FVector variable for Launch calculation
	FVector DodgeVelocity;
	
	const float DodgeCurveFloat = DodgeCurve->GetFloatValue(DashModifier);
	
	// Depending on the direction the character will be launching itself, the LaunchVelocity variable will be modified
	switch (DirectionEnum)
	{
	case EDirection::EDI_Backward:
		DodgeVelocity = -GetActorForwardVector() * (DodgePower * DodgeCurveFloat);
		break;
	case EDirection::EDI_Forward:
		DodgeVelocity = GetActorForwardVector() * (DodgePower * DodgeCurveFloat);
		break;
	default:
		// If, somehow, an invalid enumerator was passed to this function, then the code will stop here.  
		return;
	}
	
	// Reduces the DodgeModifier variable 
	constexpr float MinModifier = 0.0f;
	constexpr float MaxModifier = 1.0f;
	DashModifier = FMath::Clamp(DashModifier + DeltaSeconds, MinModifier, MaxModifier);
	
	// Safely forces the velocity of the character for the necessary frames
	CharacterVelocityInterface(DodgeVelocity * DodgeCurveFloat);
}

void APC_C_Prince::LedgeJumpInterface()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (IsValid(MoveComp) == false)
	{
		return;
	}
	
	SetMovementState(EMovementState::ECS_AirState);
	
	MoveComp->Velocity.Z = MoveComp->JumpZVelocity * 1.5f;
}


void APC_C_Prince::MoveInputInterface(const float Direction)
{
	Move(Direction);
}

void APC_C_Prince::CharacterVelocityInterface(const FVector& NewVelocity)
{
	// Checks the existence of the Character Movement Component,
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (IsValid(MoveComp) == true)
	{
		// and proceeds to modify the velocity of the character. 
		MoveComp->Velocity = NewVelocity;
	}
}

void APC_C_Prince::CharacterVelocityInterface(const EDirection AxisDirection, const float NewVelocity)
{
	// Checks the existence of the Character Movement Component, and if it's not found or valid,
	// stop processing further code.
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (IsValid(MoveComp) == false)
	{
		return;
	}
	
	// Depending on the direction, forces the Velocity of said axis.
	switch (AxisDirection)
	{
	case EDirection::EDI_X:
		MoveComp->Velocity.X = NewVelocity;
		break;
	case EDirection::EDI_Y:
		MoveComp->Velocity.Y = NewVelocity;
		break;
	case EDirection::EDI_Z:
		MoveComp->Velocity.Z = NewVelocity;
		break;
	default:
		break;
	}
}

void APC_C_Prince::Attack()
{
	const bool bDoNotAttack = ActionState == EActionState::EAct_AttackState || 
		ActionState == EActionState::EAct_SideAttackState || ActionState == EActionState::EAct_PerfectDodgeState || 
			ActionState == EActionState::EAct_HurtState || ActionState == EActionState::EAct_KOed_State;
	
	// Prevents any action taken if the character is doing a special action
	if (bDoNotAttack == true)
	{
		return;
	}
	
	FString SectionString = "";
	FName SectionName;
	
	if (ActionState == EActionState::EAct_DodgeState)
	{
		SectionString = "Dodge";
		SectionName = FName(*SectionString);
		PlayAnimMontage_Safe(AM_Attack, SectionName);
		SetActionState(EActionState::EAct_AttackState);
		TurnToStickDirection();
		return;
	}
	
	switch (MovementState)
	{
	case EMovementState::ECS_WallState:
		SectionString = "Wall";
		SectionName = FName(*SectionString);
		PlayAnimMontage_Safe(AM_Attack, SectionName);
		SetActionState(EActionState::EAct_AttackState);
		return;
	case EMovementState::ECS_LedgeState:
		SectionString = "Ledge";
		SectionName = FName(*SectionString);
		PlayAnimMontage_Safe(AM_Attack, SectionName);
		SetActionState(EActionState::EAct_AttackState);
		return;
	default:
		break;
	}
	
	if (bIsCrouched == true)
	{
		SectionString = FString::Printf(TEXT("Down_%d"), AtkCounter);
		SectionName = FName(*SectionString);
		PlayAnimMontage_Safe(AM_Attack, SectionName);
		SetActionState(EActionState::EAct_SideAttackState);
		
		AtkCounter++;
		
		if (AtkCounter >= AtkCounterMax)
		{
			AtkCounter = 0;
		}
		return;
	}
	
	if (bIsLookingUp == true)
	{
		SectionString = FString::Printf(TEXT("Up_%d"), AtkCounter);
		SectionName = FName(*SectionString);
		PlayAnimMontage_Safe(AM_Attack, SectionName);
		SetActionState(EActionState::EAct_SideAttackState);
		
		AtkCounter++;
		
		if (AtkCounter >= AtkCounterMax)
		{
			AtkCounter = 0;
		}
		return;
	}
	
	if (StickDirection != 0.0f)
	{
		SectionString = FString::Printf(TEXT("Side_%d"), AtkCounter);
		SectionName = FName(*SectionString);
		PlayAnimMontage_Safe(AM_Attack, SectionName);
		SetActionState(EActionState::EAct_SideAttackState);
		
		AtkCounter++;
		
		if (AtkCounter >= AtkCounterMax)
		{
			AtkCounter = 0;
		}
		return;
	}
	
	// Otherwise, proceed to attack as normally intended on the ground or in the air.
	Super::Attack();
}

void APC_C_Prince::LedgeUpAnim()
{
	if (MovementState == EMovementState::ECS_LedgeState && ActionState == EActionState::EAct_NilState)
	{
		SetActionState(EActionState::EAct_DodgeState);

		const FString SectionString = "Up";
		const FName SectionName = FName(*SectionString);
		PlayAnimMontage_Safe(AM_LedgeAction, SectionName);
		SetActionState(EActionState::EAct_DodgeState);
	}
}

void APC_C_Prince::LedgeDownAnim()
{
	if (MovementState == EMovementState::ECS_LedgeState && ActionState == EActionState::EAct_NilState)
	{
		SetActionState(EActionState::EAct_DodgeState);
		SetMovementState(EMovementState::ECS_AirState);
		const FString SectionString = "Down";
		const FName SectionName = FName(*SectionString);
		PlayAnimMontage_Safe(AM_LedgeAction, SectionName);
		SetActionState(EActionState::EAct_PerfectDodgeState);
	}
}

void APC_C_Prince::WallDetection()
{
	// If the character is doing an action, it's already in the MovementState of LedgeState, or it's not in the air, 
	// do not process further code in this function.
	if (ActionState != EActionState::EAct_NilState || MovementState != EMovementState::ECS_AirState)
	{
		return;
	}
	
	// Must also work only if the World and the Capsule Component of the character exists
	UWorld* World = GetWorld();
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	
	// and also only if the character is in the air
	if (IsValid(World) == false || IsValid(CapsuleComp) == false)
	{
		return;
	}
	
	// Creates the FHitResult variable
	FHitResult Hit;
	
	// Makes a constant expression for the Collision Channel to detect
	constexpr ECollisionChannel WallTraceChannel = ECollisionChannel::ECC_WorldStatic;
	
	// Makes the Collision Parameters in which the trace will ignore the character
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	/* Makes the calculations for the Line Trace to check for Static Walls */
	
	// Constant Expressions and Constant Floats
	const float WallHalfHeightMod = CapsuleComp->GetScaledCapsuleHalfHeight() * WallTraceHeightModifier;
	const float WallRadiusMod = (CapsuleComp->GetScaledCapsuleRadius()) * StickDirection;
	constexpr float WallDistanceEnd = 1.0f;
	
	/* Set and Modify Vectors */
	
	// Set the Top Trace starting position with the capsule's radius as a starting point
	FVector WallTraceTopStart = GetActorLocation();
	WallTraceTopStart.X += WallRadiusMod;
	WallTraceTopStart.Z += WallHalfHeightMod;
	
	// Create the End Location for the Top and Bottom Traces and modify their X variable according to the Stick direction 
	FVector WallTraceTopEnd = WallTraceTopStart;
	WallTraceTopEnd.X += WallDistanceEnd * StickDirection;
	
	// Set different booleans for each wall detection trace, on top and bottom halves of the character,
	bWallDetectTop = World->LineTraceSingleByChannel(Hit, WallTraceTopStart, WallTraceTopEnd,
		WallTraceChannel, CollisionParams);
	
	if (MovementState == EMovementState::ECS_WallState)
	{
		return;
	}
	
	// Take the already worked out calculation from the top starting position for the bottom starting position
	FVector WallTraceBotStart = WallTraceTopStart;
	WallTraceBotStart.Z -= WallHalfHeightMod * 2.0f;
	
	FVector WallTraceBotEnd = WallTraceBotStart;
	WallTraceBotEnd.X += WallDistanceEnd * StickDirection;
	
	const bool bWallDetectBot = World->LineTraceSingleByChannel(Hit, WallTraceBotStart, WallTraceBotEnd,
		WallTraceChannel, CollisionParams);;
	
	// and then apply both to the final result
	const bool bWallDetect = bWallDetectTop && bWallDetectBot;
	
	// But if a Wall is detected, sets the MovementState to WallState through its proper function
	if (bWallDetect == true)
	{
		TurnToStickDirection();
		SetMovementState(EMovementState::ECS_WallState);
		
		FString SectionText = "Default";
		
		UCharacterMovementComponent* MoveComp = GetCharacterMovement();
		if (IsValid(MoveComp) == true)
		{
			float UpVelocity = MoveComp->GetLastUpdateVelocity().Z;
			constexpr float UpVelocityThreshold = 300.0f;
			
			if (UpVelocity > UpVelocityThreshold)
			{
				SectionText = "Up";	
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "NOT VALID");
		}
		
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, SectionText);
		
		FName SectionName = *SectionText;
		
		PlayAnimMontage_Safe(AM_WallStart, SectionName);
		return;
	}
	
	// Settings for the Debug Drawing Line
	const FColor WallTraceColor = FColor::Red;
	
	constexpr bool bPersistentLines = false;
	constexpr float LifeTime = -1.0f;
	constexpr uint8 DepthPriority = 1;
	constexpr float Thickness = 10.0f;
	
	// Draws the Debug Line to simulate the wall detection 
	DrawDebugLine(World, WallTraceTopStart, WallTraceTopEnd, WallTraceColor, bPersistentLines, LifeTime,
		DepthPriority, Thickness);
	
	DrawDebugLine(World, WallTraceBotStart, WallTraceBotEnd, WallTraceColor, bPersistentLines, LifeTime,
		DepthPriority, Thickness);
}

void APC_C_Prince::LedgeDetection()
{
	// When the too side of the trace detects a wall, proceed to check if the character can be ledged
	if (bWallDetectTop == false || MovementState == EMovementState::ECS_LedgeState || 
		ActionState == EActionState::EAct_WallJumpState)
	{
		return;
	}
	
	// Must also work only if the World and the Capsule Component of the character exists
	UWorld* World = GetWorld();
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	
	// and also only if the character is in the air
	if (IsValid(World) == false || IsValid(CapsuleComp) == false)
	{
		return;
	}
	
	// Creates the FHitResult variable
	FHitResult Hit;
	
	// Makes a constant expression for the Collision Channel to detect
	constexpr ECollisionChannel WallTraceChannel = ECollisionChannel::ECC_WorldStatic;
	
	// Makes the Collision Parameters in which the trace will ignore the character
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	// Prepares fewer settings for the detection of the short trace
	constexpr float TraceDistanceEnd = 1.0f;
	constexpr float ExtraHeight = 15.0f;
	const float CharacterDirection = GetActorForwardVector().X; 
	const float WallHalfHeightMod = CapsuleComp->GetScaledCapsuleHalfHeight() * WallTraceHeightModifier;
	const float WallRadiusMod = (CapsuleComp->GetScaledCapsuleRadius()) * StickDirection;
	
	// Sets the Starting and Ending trace positions using the already established top trace position for the wall trace
	// Set the Top Trace starting position with the capsule's radius as a starting point
	FVector LedgeTraceStart = GetActorLocation();
	LedgeTraceStart.X += WallRadiusMod;
	LedgeTraceStart.Z += WallHalfHeightMod + ExtraHeight;
	FVector LedgeTraceEnd = LedgeTraceStart;
	LedgeTraceEnd.X += TraceDistanceEnd * StickDirection;
	
	// Set the new boolean to the opposite of it's hit detection; in other words, if the trace does NOT detect a wall
	// near and above the character's head, then that means that the character can ledge grab it.
	const bool LedgeDetected = !World->LineTraceSingleByChannel(Hit, LedgeTraceStart, LedgeTraceEnd,
		WallTraceChannel, CollisionParams);
	
	// Settings for the Debug Drawing Line
	const FColor WallTraceColor = FColor::Green;
	
	constexpr bool bPersistentLines = true;
	constexpr float LifeTime = 2.0f;
	constexpr uint8 DepthPriority = 1;
	constexpr float Thickness = 10.0f;
	
	// Draws the Debug Line to simulate the wall detection 
	DrawDebugLine(World, LedgeTraceStart, LedgeTraceEnd, WallTraceColor, bPersistentLines, LifeTime,
		DepthPriority, Thickness);
	
	// If a ledge is detected, set the proper rotation and also the movement state to LedgeState, and play the
	// corresponding AnimMontage for ledge grabbing
	if (LedgeDetected == true)
	{
		TurnToStickDirection();
		SetMovementState(EMovementState::ECS_LedgeState);
		PlayAnimMontage_Safe(AM_LedgeStart);
		SetActionState(EActionState::EAct_PerfectDodgeState);
		SetWallDetectTop(false);
	}
}

void APC_C_Prince::TurnToStickDirection()
{
	// Prepare the Constant Expression Variables
	constexpr float Zero = 0.0f;
	constexpr float Turn = 180.0f;
	
	// If the Stick is pushed to the Right, set the rotation to Zero
	if (StickDirection > Zero)
	{
		SetActorRotation(FRotator(Zero, Zero, Zero));
	}
	// Otherwise, if the Stick if pushed to the Left, turn the Character 180 degrees to face the left side of the screen
	else if (StickDirection < Zero)
	{
		SetActorRotation(FRotator(Zero, Turn, Zero));
	} 
	// Also, if the Player is not pushing the Stick to influence movement
	else if (FMath::IsNearlyZero(StickDirection, 0.01f) == true)
	{
		// Obtain the Character Movement Component
		const UCharacterMovementComponent* MoveComp = GetCharacterMovement();
		if (IsValid(MoveComp) == false)
		{
			return;
		}
		// and check at which side the character if moving without the influence of the player,
		const float Direction = MoveComp->GetLastUpdateVelocity().X;
		// And according to the direction, turn the character's rotation to where it should be facing.
		if (Direction > Zero)
		{
			SetActorRotation(FRotator(Zero, Zero, Zero));
		} 
		else if (Direction < Zero)
		{
			SetActorRotation(FRotator(Zero, Turn, Zero));
		} 
	}
}

void APC_C_Prince::CrouchStart()
{
	if (ActionState == EActionState::EAct_NilState)
	{
		// If the character is in a state where it's standing on the ground,
		if (MovementState == EMovementState::ECS_GroundState)
		{
			// Crouch.
			Crouch();
		}
	}
}

void APC_C_Prince::CrouchEnd()
{
	if (ActionState == EActionState::EAct_NilState)
	{
		// If the current state is crouching,
		if (MovementState == EMovementState::ECS_CrouchState)
		{
			// UnCrouch.
			UnCrouch();
		}
	}
}

void APC_C_Prince::DodgeFromAnimation()
{
	// As long as the character is not doing any special action, proceed
	if (ActionState != EActionState::EAct_NilState)
	{
		return;
	}
	
	// Ends the crouch in case the character was crouching.
	CrouchEnd();
	
	// Creates the variable for the Anim Montage's Section Name
	FName SectionName;
	
	// If the stick is NOT being pushed in the horizontal axis,
	if (StickDirection == 0.0)
	{
		// Call the animation with the section named "Back" to back dash on the ground,
		SectionName = FName("Back");
	}
	else
	{
		// otherwise, the section called is "Fwd" to forward dash on the ground
		SectionName = FName("Fwd");
	}
	
	// If the character was in a state of Wall Sliding, then turn around and proceed to dash.
	if (MovementState == EMovementState::ECS_WallState || MovementState == EMovementState::ECS_LedgeState)
	{
		UCharacterMovementComponent* MoveComp = GetCharacterMovement();
		if (IsValid(MoveComp) == false)
		{
			return;
		}
		
		SetWallDetectTop(false);
		
		// Change back the MovementState to AirState so the character is not stuck in the WallState
		SetMovementState(EMovementState::ECS_AirState);
		
		// Makes the character stop falling for a second and turns the character around
		constexpr float UpSpeed = 200.0f;
		constexpr EDirection Axis = EDirection::EDI_Z;
		
		CharacterVelocityInterface(Axis, UpSpeed);
		
		SectionName = FName("Wall");
	}
	
	// Sets the movement state to Dodge so the behaviors on damage reception apply
	SetActionState(EActionState::EAct_PerfectDodgeState);
	
	// and plays the Dodge AnimMontage with the established section name.
	PlayAnimMontage_Safe(AM_Dodge, SectionName);
}

/// ReSharper disable once CppMemberFunctionMayBeConst
void APC_C_Prince::Interact()
{
	if (ActorToInteract.IsValid() == false)
	{
		return;
	}
	
	if (IInteractions* InteractInterface = Cast<IInteractions>(ActorToInteract.Get()))
	{
		InteractInterface->InteractWith();
	}
}

void APC_C_Prince::CoyoteJumpDisable()
{
	CoyoteJumpCurrentCount = 1;
	
	PrintDebug("CoyoteJumpDisable");
}

void APC_C_Prince::DeactivateFromAnim()
{
	SetActionState(EActionState::EAct_PerfectDodgeState);
	
	PlayAnimMontage_Safe(AM_SwitchAnim);
}

void APC_C_Prince::ActivateCharacter()
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->SetMovementMode(MOVE_Walking);
	}
	
	HitState = EHitState::EHit_Damage;
	
	if (USkeletalMeshComponent* SkeletalMeshComponent = GetMesh())
	{
		SkeletalMeshComponent->SetVisibility(true);
		SkeletalMeshComponent->Activate(true);
	}
}

void APC_C_Prince::DeactivateCharacter()
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->SetMovementMode(MOVE_None);
	}
	
	HitState = EHitState::EHit_NoDamage;
	
	if (USkeletalMeshComponent* SkeletalMeshComponent = GetMesh())
	{
		SkeletalMeshComponent->SetVisibility(false);
		SkeletalMeshComponent->Deactivate();
	}
	
	constexpr float Zero = 0.0f;
	constexpr float Distance = -1000.0f;
	
	SetActorLocation(FVector(Zero, Zero, Distance));
	
	StickDirection = 0.0f;
	
	SetSpeedState(ESpeedState::ESP_Stop);
}

