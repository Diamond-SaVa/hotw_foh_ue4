// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HOTW_FOH_UE4_API IDamageable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Damage tracing function from sphere/capsule shape
	virtual void SphereTraceDamage(const FVector& StartLocation, const FVector& EndLocation, 
		TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, float Radius = 20.0f) = 0;
	
	// Damage taking function
	UFUNCTION(BlueprintCallable, Category = "Damage Functions")
	virtual void TakeDamage(const int32 Damage) = 0;
	
	// Knockback launch Function
	UFUNCTION(BlueprintCallable, Category = "Character Behavior Functions")
	virtual void KnockbackFromSource(const FVector& DamageSourceLocation, const float KnockbackPower) = 0;
};
