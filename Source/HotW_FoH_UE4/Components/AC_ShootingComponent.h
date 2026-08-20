// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	float BounceSpeed = 500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectiles")
	// Sets the Gravity Scale for the next projectile 
	float GravityChange = 0.0f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HOTW_FOH_UE4_API UAC_ShootingComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectiles")
	TArray<AActor*> ProjectileArray;

	// Timer for the Shooting Restart Function
	FTimerHandle CooldownTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectiles")
	FProjectileData NextProjectileData;
	
	// 0 if it can shoot, 1 if it cannot.
	int32 CanShoot;
	
public:
	// Sets default values for this component's properties
	UAC_ShootingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	void ShootNextAvailableProjectile();
	
	void RestartShoot();
};
