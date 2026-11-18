// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HotW_FoH_UE4/Interfaces/StateSetters.h"
#include "AC_ShootingComponent.generated.h"


USTRUCT(BlueprintType)
struct FProjectileData
{
	GENERATED_BODY()
	
	// Sets the Movement speed for the next projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectiles")
	float MovementSpeed = 500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectiles")
	// Sets the Bounce speed for the next projectile 
	float LifeTime = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectiles")
	// Sets the Gravity Scale for the next projectile 
	float GravityChange = 0.0f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HOTW_FOH_UE4_API UAC_ShootingComponent : public UActorComponent, public IStateSetters
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectiles")
	TArray<AActor*> ProjectileArray;
	
	UPROPERTY(EditAnywhere, Category = "Character Load Data")
	TArray<TSoftClassPtr<class AA_Projectile>> ClassProjectileByMaskArray;

	// Timer for the Shooting Restart Function
	FTimerHandle CooldownTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectiles")
	FProjectileData NextProjectileData;
	
	// 0 if it can shoot, 1 if it cannot.
	bool bCanShoot;
	
	EMaskState CurrentMaskFilter;
	
public:
	// Sets default values for this component's properties
	UAC_ShootingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	void ShootAction(int32 AttackValue);
	
	void SpawnNextProjectileIfNeeded(UWorld* World, int32 AttackValue);
	
	void ShootNextAvailableProjectile(UWorld* World, AActor* NextProjectile, int32 AttackValue);
	
	void RestartShoot();
	
	/* State Setters Interface Functions */
	
	virtual void SetMaskState(const EMaskState NewState) override;
	
	virtual void SetActionState(const EActionState NewState) override;
	
	virtual void SetBoolGoingUp(const bool IsGoingUp) override;
	
	virtual void SetMovementState(const EMovementState NewState) override;
	
	virtual void SetProjectileOfMask(const TSoftClassPtr<class AA_Projectile>& NewProjectile) override;
	
	virtual void SetSpeedState(const ESpeedState NewState) override;
	
	/* Getters and Setters */
	
	FORCEINLINE bool GetCanShoot() const { return bCanShoot; }
};
