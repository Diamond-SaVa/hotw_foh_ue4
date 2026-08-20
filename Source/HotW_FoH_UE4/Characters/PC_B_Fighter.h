// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "PC_A_Base.h"
#include "HotW_FoH_UE4/Interfaces/Damageable.h"
#include "PC_B_Fighter.generated.h"

class UAC_Stats;

UCLASS()
class HOTW_FOH_UE4_API APC_B_Fighter : public APC_A_Base, public IDamageable
{
	GENERATED_BODY()
	
protected:
	// Stats Component for Character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Stats")
	UAC_Stats* StatsComponent;
	
	// Animation Montage for Character's Attack Animations
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Assets")
	UAnimMontage* AM_Attack;
	
	// Animation Montage for Character's Damage Animations
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Assets")
	UAnimMontage* AM_Damage;
	
	// Attack Count
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack Properties")
	int32 AtkCounter;
	
	// Attack Count
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Properties")
	int32 AtkCounterMax = 1;
	
	// Remembers if the character can or can't be hit or hurt
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	EHitState HitState;
	
	// Bool to check if looking up
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement Data")
	bool bIsLookingUp = false;
	
	// Bool to check if looking down
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement Data")
	bool bIsLookingDown = false;

public:
	// Sets default values for this character's properties
	APC_B_Fighter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	virtual void Landed(const FHitResult& Hit) override;
	
	/* Pure C++ Interface Functions Implementations */
	
	/// Interface for Damage reception only.
	/// @param Damage Amount of damage in Integer values
	virtual void TakeDamage(const int32 Damage) override;

	/// Interface for Knockback reception only
	/// @param DamageSourceLocation Location from where the character was hit 
	/// @param KnockbackPower Knockback for Launching characters
	virtual void KnockbackFromSource(const FVector& DamageSourceLocation, const float KnockbackPower) override;
	
	virtual void SphereTraceDamage(const FVector& StartLocation, const FVector& EndLocation, 
		TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const float Radius = 20.0f) override;
	
	/* Public functions for access from AI Trees */
public:
	/// Attack function that can be accessed by Events from AI Trees when it applies.
	virtual void Attack();
	
	FORCEINLINE bool GetIsLookingUp() const { return bIsLookingUp; }
	
	FORCEINLINE void SetIsLookingUp(const bool NewLook) { bIsLookingUp = NewLook; }
	
	FORCEINLINE bool GetIsLookingDown() const { return bIsLookingDown; }
	
	FORCEINLINE void SetIsLookingDown(const bool NewLook) { bIsLookingDown = NewLook; }
};
