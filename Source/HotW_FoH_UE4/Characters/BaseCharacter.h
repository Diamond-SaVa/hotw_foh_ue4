// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HotW_FoH_UE4/Interfaces/Damageable.h"
#include "HotW_FoH_UE4/Interfaces/Movement.h"
#include "HotW_FoH_UE4/Interfaces/StateSetters.h"
#include "BaseCharacter.generated.h"

UCLASS()
class HOTW_FOH_UE4_API ABaseCharacter : public ACharacter, public IStateSetters, public IMovement, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/* Virtual functions from Character Class */
	
	virtual void OnJumped_Implementation() override;
	
	virtual void Landed(const FHitResult& Hit) override;
	
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	
	/* Virtual functions for all Actions coming from Controller */
	
	virtual void InteractStart();
	
	virtual void MoveStart(float Direction);
	
	virtual void MoveHold(float Direction);
	
	virtual void MoveStop();
	
	virtual void JumpStart();
	
	virtual void JumpHold();
	
	virtual void JumpEnd();
	
	virtual void AttackStart();
	
	virtual void AttackHold();
	
	virtual void AttackEnd();
	
	virtual void DodgeStart();
	
	virtual void DodgeHold();
	
	virtual void DodgeEnd();
	
	virtual void ActivateCharacter(AController* NewController);
	
	virtual void DeactivateCharacter();
	
	/* Functions called from Wall and Ledge Component */
	
	virtual void OnLedgeGrabStart();
	
	virtual void OnLedgeGrabStop();
	
	virtual void OnWallSlideStart();
	
	virtual void OnWallSlideStop();
	
	/* Virtual Interface Functions from IStateSetters */
	
	virtual void SetMovementState(const EMovementState NewState) override;
	
	virtual void SetActionState(const EActionState NewState) override;
	
	virtual void SetSpeedState(const ESpeedState NewState) override;
	
	virtual void SetBoolGoingUp(const bool IsGoingUp) override;
	
	/* Virtual Interface Functions from IMovement */
	
	// Send from Interface where the character should move with physics.
	// Can be used with Tick.
	virtual void CharacterVelocityInterface(const FVector& NewVelocity) override;
	
	// Send from Interface in which direction the character will be moved with physics in relation to
	// their current rotation. Can be used with Tick
	virtual void CharacterVelocityInterface(const EDirection AxisDirection, const float NewVelocity = 0.0f) override;
	
	// Using the power of LaunchCharacter, the character is launched in the respective direction in correlation to
	// the character's rotation. Not to be used in Tick.
	virtual void DodgeInterface(const float DodgePower, const float DeltaSeconds, const EDirection DirectionEnum) override;
	
	// Used from Wall and Ledge Component.
	// Calls to Character's behavior to change as if 
	virtual void LedgeGrabStart() override;
	
	virtual void LedgeJumpInterface() override;
	
	virtual void MoveInputInterface(const float Direction) override;
	
	virtual void WallSlideStart() override;
	
	/* Virtual Interface Functions from IDageable */
	
	virtual void SphereTraceDamage(const FVector& StartLocation, const FVector& EndLocation, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, float Radius = 20) override;
	
	virtual void TakeDamage(const int32 Damage) override;
	
	virtual void KnockbackFromSource(const FVector& DamageSourceLocation, const float KnockbackPower) override;
	
};
