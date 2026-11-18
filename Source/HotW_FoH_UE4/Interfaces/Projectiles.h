// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "HotW_FoH_UE4/Enums/ECharacterEnums.h"
#include "UObject/Interface.h"
#include "Projectiles.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UProjectiles : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HOTW_FOH_UE4_API IProjectiles
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, Category="Projectile Behavior")
	virtual void ActivateProjectile(AActor* ActorSource, float MovementSpeedChange, float NewAttackValue,
									float GravityChange, float LifeTime) = 0;
	
	UFUNCTION(BlueprintCallable, Category = "Getters and Setters")
	virtual EMaskState GetProjectileForMaskType() = 0;
	
	UFUNCTION(BlueprintCallable, Category="Getters and Setters")
	virtual bool GetProjectileIsReady() = 0;
};
