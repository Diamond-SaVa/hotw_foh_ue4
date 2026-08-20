// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_ShootingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HOTW_FOH_UE4_API UAC_ShootingComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectiles")
	TArray<AActor*> ProjectileArray;

	// Timer for the Shooting Restart Function
	FTimerHandle TimerHandle;
	
	// 0 if it can shoot, 1 if it cannot.
	int32 CanShoot;
	
public:
	// Sets default values for this component's properties
	UAC_ShootingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	void ShootNextAvailableProjectile();
	
	void RestartShoot();
};
