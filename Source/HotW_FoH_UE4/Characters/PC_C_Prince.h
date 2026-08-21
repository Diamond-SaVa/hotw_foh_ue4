// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "PC_B_Fighter.h"
#include "HotW_FoH_UE4/Interfaces/Movement.h"
#include "PC_C_Prince.generated.h"

class UCurveFloat;

UCLASS()
class HOTW_FOH_UE4_API APC_C_Prince : public APC_B_Fighter, public IMovement
{
	GENERATED_BODY()
	
protected:	
	FTimerHandle CoyoteTimerHandle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement Data")
	class UAC_WallAndLedgeComponent* WallSlideComponent;
	
	// Animation for Interactions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Assets")
	UAnimMontage* AM_Interaction;
	
	// Animation for Dodging
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Assets")
	UAnimMontage* AM_Dodge;
	
	// Animation for rising from crouch
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Assets")
	UAnimMontage* AM_SwitchAnim;
	
	// Pointer to a Curve to use for Dodge Velocity overtime
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Data")
	UCurveFloat* DodgeCurve;
	
	// Weak pointer to an Actor to interact with if player enters in range of an NPC or interactable object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction Data")
	TWeakObjectPtr<AActor> ActorToInteract;
	
	// Dodge Power Modifier reduction
	float DashModifier = 0.0f;
	
	// Modifier of the Height at which the character detects a wall from
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall Trace Data", 
		meta=(ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float WallTraceHeightModifier = 0.5f;
	
	float HalfHeightMemory;
	
	int32 CoyoteJumpCurrentCount = 0;
	
	// ID for specific playable characters. 0 is for the prince. 1 is for the girl.
	int32 CharacterID = 0;
	
	bool bWallDetectTop;

public:
	// Sets default values for this character's properties
	APC_C_Prince();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Overriden OnMovementModeChanged
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	
	// Overrides to detect leaving the ledge
	virtual void OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, 
		const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta) override;

protected:
	virtual void Landed(const FHitResult& Hit) override;
	
	// Event called when character begins to crouch
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	
	// Event called when character stops crouching
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
		
public:
	/* Overriden Functions from Base Class */
	
	// Overriden function from base SetMovementState virtual function
	virtual void SetMovementState(const EMovementState NewState) override;
	
	// Overriden function from base SetActionState virtual function
	virtual void SetActionState(const EActionState NewState) override;

	// Overriden function to Jump behaviors before going through the super class
	virtual void JumpStart() override;
	
protected:
	// Callback with the correct signature for OnMontageEnded
	UFUNCTION()
	void HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
public:
	/* Pure C++ Interfaces */
	// Character Dodge from AnimStateNotifies or inner calls
	virtual void DodgeInterface(const float DodgePower, const float DeltaSeconds, const EDirection DirectionEnum) override;
	
	virtual void LedgeJumpInterface() override;
	
	virtual void MoveInputInterface(const float Direction) override;
	
	// Safely forces the Character's Velocity with new FVector
	virtual void CharacterVelocityInterface(const FVector& NewVelocity) override;
	
	// Safely forces only a specific Axis of the Character's Velocity
	virtual void CharacterVelocityInterface(const EDirection AxisDirection, const float NewVelocity = 0.0f) override;
	
	virtual void Attack() override;
	
	virtual void LedgeUpAnim();
	
	virtual void LedgeDownAnim();
	
	void TurnToStickDirection();
	
	// Function for Crouch Start
	void CrouchStart();
	
	// Function for Crouch End
	void CrouchEnd();
	
	// Function to manage the dodge behavior of the character
	void DodgeFromAnimation();
	
	// Function to make character interact with the ActorToInteract variable.
	void Interact();
	
	void CoyoteJumpDisable();

	void DeactivateFromAnim();
	
	void ActivateCharacter();
	
	void DeactivateCharacter();

	FORCEINLINE bool GetWallDetectTop() const { return bWallDetectTop; }
	
	FORCEINLINE void SetWallDetectTop(const bool NewDetect) { bWallDetectTop = NewDetect; }
};
