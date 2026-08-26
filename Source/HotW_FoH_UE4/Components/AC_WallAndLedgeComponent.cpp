// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "AC_WallAndLedgeComponent.h"

#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "HotW_FoH_UE4/Characters/PC_C_Prince.h"
#include "HotW_FoH_UE4/Interfaces/Movement.h"

// Sets default values for this component's properties
UAC_WallAndLedgeComponent::UAC_WallAndLedgeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAC_WallAndLedgeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	// Start the life cyicle with Tick disabled
	SetComponentTickEnabled(false);
	
	ACharacter* PCOwner = Cast<ACharacter>(GetOwner());
	
	if (PCOwner == nullptr)
	{
		return;
	}

	/* Make the calculations for the Line Trace to check for Static Walls */
	UCapsuleComponent* CapsuleComp = PCOwner->GetCapsuleComponent();
	if (CapsuleComp == nullptr)
	{
		return;
	}
	
	WallHalfHeightMod = CapsuleComp->GetScaledCapsuleHalfHeight() * WallTraceHeightModifier;
	WallRadiusMod = CapsuleComp->GetScaledCapsuleRadius();
}


// Called every frame
void UAC_WallAndLedgeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
	WallDetection();
	LedgeDetection();
}

void UAC_WallAndLedgeComponent::SetMovementState(const EMovementState NewState)
{
	CompMovementState = NewState;
	
	switch (NewState)
	{
	case EMovementState::ECS_AirState:
	case EMovementState::ECS_WallState:
		SetComponentTickEnabled(true);
		break;
	case EMovementState::ECS_LedgeState:
	default:
		SetComponentTickEnabled(false);
		break;
	}
}

void UAC_WallAndLedgeComponent::SetActionState(const EActionState NewState)
{
	CompActionState = NewState;
}

void UAC_WallAndLedgeComponent::SetSpeedState(const ESpeedState NewState)
{
	/* KEEP EMPTY */
}

void UAC_WallAndLedgeComponent::SetBoolGoingUp(const bool IsGoingUp)
{
	/* KEEP EMPTY */
}

void UAC_WallAndLedgeComponent::WallDetection()
{
	/**/
	// If the character is doing an action, it's already in the MovementState of LedgeState, or it's not in the air, 
	// do not process further code in this function.
	if (CompActionState != EActionState::EAct_NilState || CompMovementState != EMovementState::ECS_AirState)
	{
		if (CompMovementState == EMovementState::ECS_WallState)
		{
			AActor* PCOwner = Cast<AActor>(GetOwner());
			UWorld* World = GetWorld();
			if (World == nullptr || PCOwner == nullptr)
			{
				return;
			}
			bool bStillOnWall = true;
			TraceFunction(PCOwner, World, false, bStillOnWall);
			
			if (bStillOnWall == false)
			{
				if (IStateSetters* OwnerSetters = Cast<IStateSetters>(PCOwner))
				{
					OwnerSetters->SetMovementState(EMovementState::ECS_AirState);
				}
			}
		}
		
		return;
	}
	/**/
	
	// Must also work only if the World and the Owner of the Component exist
	AActor* PCOwner = Cast<AActor>(GetOwner());
	UWorld* World = GetWorld();
	if (World == nullptr || PCOwner == nullptr)
	{
		return;
	}
	
	TraceFunction(PCOwner, World, true, bWallDetectTop);
	
	bool bWallDetectBot = false;
	
	TraceFunction(PCOwner, World, false, bWallDetectBot);
	
	// But if a Wall is detected, sets the MovementState to WallState through its proper function
	if (bWallDetectTop == true && bWallDetectBot == true)
	{
		if (IMovement* Movement = Cast<IMovement>(PCOwner))
		{
			Movement->WallSlideStart();
		}
	}
}

void UAC_WallAndLedgeComponent::LedgeDetection()
{
	// When the too side of the trace detects a wall, proceed to check if the character can be ledged
	if (bWallDetectTop == false || CompActionState != EActionState::EAct_NilState || 
		CompActionState == EActionState::EAct_WallJumpState)
	{
		return;
	}
	
	// Must also work only if the World and the Capsule Component of the character exists
	UWorld* World = GetWorld();
	AActor* PCOwner = GetOwner();
	if (World == nullptr || PCOwner == nullptr || 
		(CompMovementState == EMovementState::ECS_WallState && PCOwner->GetVelocity().Z < 0.0f))
	{
		return;
	}
	
	// Creates the FHitResult variable
	FHitResult Hit(ForceInit);
	
	// Makes a constant expression for the Collision Channel to detect
	constexpr ECollisionChannel WallTraceChannel = ECollisionChannel::ECC_WorldStatic;
	
	// Makes the Collision Parameters in which the trace will ignore the character
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(PCOwner);
	
	// Prepares fewer settings for the detection of the short trace
	constexpr float TraceDistanceEnd = 1.0f;
	constexpr float ExtraHeight = 15.0f;
	const float CharacterDirection = FMath::RoundToFloat(PCOwner->GetActorForwardVector().X);
	
	// Sets the Starting and Ending trace positions using the already established top trace position for the wall trace
	// Set the Top Trace starting position with the capsule's radius as a starting point
	FVector LedgeTraceStart = PCOwner->GetActorLocation();
	LedgeTraceStart.X += WallRadiusMod * CharacterDirection;
	LedgeTraceStart.Z += WallHalfHeightMod + ExtraHeight;
	FVector LedgeTraceEnd = LedgeTraceStart;
	LedgeTraceEnd.X += TraceDistanceEnd * CharacterDirection;
	
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
		if (IMovement* Movement = Cast<IMovement>(PCOwner))
		{
			Movement->LedgeGrabStart();
			bWallDetectTop = false;
		}
	}
}

void UAC_WallAndLedgeComponent::TraceFunction(AActor* PCOwner, UWorld* World, const bool bTopTrace, bool& bTraceResult)
{
	// Creates the FHitResult variable
	FHitResult Hit;
	
	// Makes a constant expression for the Collision Channel to detect
	constexpr ECollisionChannel WallTraceChannel = ECollisionChannel::ECC_WorldStatic;
	
	// Makes the Collision Parameters in which the trace will ignore the character
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(PCOwner);
	
	// Constant Expressions and Constant Floats
	constexpr float WallDistanceEnd = 1.0f;
	const float CharacterDirection = FMath::RoundToFloat(PCOwner->GetActorForwardVector().X);
	
	/* Set and Modify Vectors */
	
	// Set the Top Trace starting position with the capsule's radius as a starting point
	FVector WallTraceStart = PCOwner->GetActorLocation();
	WallTraceStart.X += WallRadiusMod * CharacterDirection;
	
	if (bTopTrace == true)
	{
		WallTraceStart.Z += WallHalfHeightMod;
	}
	else
	{
		WallTraceStart.Z -= WallHalfHeightMod;
	}
		
	// Create the End Location for the Top and Bottom Traces and modify their X variable according to the Stick direction 
	FVector WallTraceEnd = WallTraceStart;
	WallTraceEnd.X += WallDistanceEnd * CharacterDirection;
	
	// Set the top wall detection according to the Trace's Hit
	bTraceResult = World->LineTraceSingleByChannel(Hit, WallTraceStart, WallTraceEnd,
		WallTraceChannel, CollisionParams);
	
	// Settings for the Debug Drawing Line
	const FColor WallTraceColor = FColor::Red;
	
	constexpr bool bPersistentLines = false;
	constexpr float LifeTime = -1.0f;
	constexpr uint8 DepthPriority = 1;
	constexpr float Thickness = 10.0f;
	
	DrawDebugLine(World, WallTraceStart, WallTraceEnd, WallTraceColor, bPersistentLines, LifeTime,
		DepthPriority, Thickness);
}

void UAC_WallAndLedgeComponent::ResetOnLanded()
{
	bWallDetectTop = false;
}
