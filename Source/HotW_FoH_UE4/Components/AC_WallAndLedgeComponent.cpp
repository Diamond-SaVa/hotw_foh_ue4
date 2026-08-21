// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "AC_WallAndLedgeComponent.h"

#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HotW_FoH_UE4/Characters/PC_C_Prince.h"


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
	
	PCOwner = Cast<APC_C_Prince>(GetOwner());
	
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

void UAC_WallAndLedgeComponent::WallDetection()
{
	// If the character is doing an action, it's already in the MovementState of LedgeState, or it's not in the air, 
	// do not process further code in this function.
	if (PCOwner == nullptr ||
		PCOwner->GetActionState() != EActionState::EAct_NilState || 
		PCOwner->GetMovementState() != EMovementState::ECS_AirState)
	{
		return;
	}
	
	// Must also work only if the World and the Capsule Component of the character exists
	UWorld* World = GetWorld();
	// and also only if the character is in the air
	if (IsValid(World) == false)
	{
		return;
	}
	
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
	FVector WallTraceTopStart = PCOwner->GetActorLocation();
	WallTraceTopStart.X += WallRadiusMod * CharacterDirection;
	WallTraceTopStart.Z += WallHalfHeightMod;
	
	// Create the End Location for the Top and Bottom Traces and modify their X variable according to the Stick direction 
	FVector WallTraceTopEnd = WallTraceTopStart;
	WallTraceTopEnd.X += WallDistanceEnd * CharacterDirection;
	
	// Set different booleans for each wall detection trace, on top and bottom halves of the character,
	const bool bWallDetectTop = World->LineTraceSingleByChannel(Hit, WallTraceTopStart, WallTraceTopEnd,
		WallTraceChannel, CollisionParams);
	
	if (bWallDetectTop == true)
	{
		IsDetectingWallFromTop = 1;
	}
	else
	{
		IsDetectingWallFromTop = 0;
	}
	
	if (PCOwner->GetMovementState() == EMovementState::ECS_WallState)
	{
		return;
	}
	
	// Take the already worked out calculation from the top starting position for the bottom starting position
	FVector WallTraceBotStart = WallTraceTopStart;
	WallTraceBotStart.Z -= WallHalfHeightMod * 2.0f;
	
	FVector WallTraceBotEnd = WallTraceBotStart;
	WallTraceBotEnd.X += WallDistanceEnd * CharacterDirection;
	
	const bool bWallDetectBot = World->LineTraceSingleByChannel(Hit, WallTraceBotStart, WallTraceBotEnd,
		WallTraceChannel, CollisionParams);;
	
	// and then apply both to the final result
	const bool bWallDetect = bWallDetectTop && bWallDetectBot;
	
	// But if a Wall is detected, sets the MovementState to WallState through its proper function
	if (bWallDetect == true)
	{
		PCOwner->TurnToStickDirection();
		PCOwner->SetMovementState(EMovementState::ECS_WallState);
		
		FString SectionText = "Default";
		
		UCharacterMovementComponent* MoveComp = PCOwner->GetCharacterMovement();
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
		
		PCOwner->PlayAnimMontage_Safe(AM_WallStart, SectionName);
		
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

void UAC_WallAndLedgeComponent::LedgeDetection()
{
	// When the too side of the trace detects a wall, proceed to check if the character can be ledged
	if (PCOwner == nullptr || IsDetectingWallFromTop == 0 || 
		PCOwner->GetMovementState() == EMovementState::ECS_LedgeState || 
		PCOwner->GetActionState() != EActionState::EAct_NilState)
	{
		return;
	}
	
	// Must also work only if the World and the Capsule Component of the character exists
	UWorld* World = GetWorld();
	UCapsuleComponent* CapsuleComp = PCOwner->GetCapsuleComponent();
	
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
		PCOwner->TurnToStickDirection();
		PCOwner->SetMovementState(EMovementState::ECS_LedgeState);
		PCOwner->PlayAnimMontage_Safe(AM_LedgeStart);
		PCOwner->SetActionState(EActionState::EAct_PerfectDodgeState);
		PCOwner->SetWallDetectTop(false);
	}
}

void UAC_WallAndLedgeComponent::ResetOnLanded()
{
	IsDetectingWallFromTop = 0;
}

void UAC_WallAndLedgeComponent::LedgeUpAnim()
{
	if (PCOwner->GetMovementState() == EMovementState::ECS_LedgeState && 
		PCOwner->GetActionState() == EActionState::EAct_NilState)
	{
		PCOwner->SetActionState(EActionState::EAct_DodgeState);

		const FString SectionString = "Up";
		const FName SectionName = FName(*SectionString);
		PCOwner->PlayAnimMontage_Safe(AM_LedgeAction, SectionName);
		PCOwner->SetActionState(EActionState::EAct_DodgeState);
	}
}

void UAC_WallAndLedgeComponent::LedgeDownAnim()
{
	if (PCOwner->GetMovementState() == EMovementState::ECS_LedgeState && 
		PCOwner->GetActionState() == EActionState::EAct_NilState)
	{
		PCOwner->SetActionState(EActionState::EAct_DodgeState);
		PCOwner->SetMovementState(EMovementState::ECS_AirState);
		const FString SectionString = "Down";
		const FName SectionName = FName(*SectionString);
		PCOwner->PlayAnimMontage_Safe(AM_LedgeAction, SectionName);
		PCOwner->SetActionState(EActionState::EAct_PerfectDodgeState);
	}
}