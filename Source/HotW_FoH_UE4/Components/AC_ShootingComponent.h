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
class HOTW_FOH_UE4_API UAC_ShootingComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Transient)
	TArray<AActor*> ProjectileArray;
	
	UPROPERTY(EditAnywhere, Category = "Character Load Data")
	TArray<TSoftClassPtr<AA_Projectile>> ClassProjectileByMaskArray;

	// Timer for the Shooting Restart Function
	FTimerHandle CooldownTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectiles")
	FProjectileData NextProjectileData;
	
	UPROPERTY(Transient)
	TSubclassOf<AA_Projectile> LoadedProjectileClass;
	
	UPROPERTY(EditAnywhere, Category="Projectiles")
	uint8 MaxShots = 4;
	
	// 0 if it can shoot, 1 if it cannot.
	bool bCanShoot = true;
	
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
	
	void ShootNextAvailableProjectile(const UWorld* World, AActor* NextProjectile, int32 AttackValue);
	
	void RestartShoot();
	
	void SetMaskAndProjectile(EMaskState NewMaskState);
	
	/* Getters and Setters */
	
	FORCEINLINE bool GetCanShoot() const { return bCanShoot; }
};
