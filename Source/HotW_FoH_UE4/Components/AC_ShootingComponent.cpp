// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "AC_ShootingComponent.h"

#include "HotW_FoH_UE4/Actors/A_Projectile.h"


// Sets default values for this component's properties
UAC_ShootingComponent::UAC_ShootingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAC_ShootingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAC_ShootingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (UWorld* World = GetWorld())
	{
		FTimerManager& WorldTimer = World->GetTimerManager();
		
		if (WorldTimer.IsTimerActive(CooldownTimerHandle))
		{
			WorldTimer.ClearTimer(CooldownTimerHandle);
		}
	}
}

void UAC_ShootingComponent::ShootNextAvailableProjectile()
{
	if (CanShoot == 1 || ProjectileArray.Num() == 0)
	{
		return;
	}

	for (AActor* Projectile : ProjectileArray)
	{
		if (IsValid(Projectile) == false)
		{
			return;
		}
		
		if (AA_Projectile* Shoot = Cast<AA_Projectile>(Projectile))
		{
			if (Shoot->ProjectileIsActive == 0)
			{
				Shoot->ActivateProjectile(GetOwner(), NextProjectileData.MovementSpeed, 
					NextProjectileData.BounceSpeed, NextProjectileData.GravityChange);
				
				CanShoot = 1;
				
				if (UWorld* World = GetWorld())
				{
					FTimerManager& WorldTimer = World->GetTimerManager();
					
					WorldTimer.SetTimer(
						CooldownTimerHandle,              // The tracking handle
						this,                     // The context object running the function
						&UAC_ShootingComponent::RestartShoot, // The address of your function
						0.5f,                     // Time in seconds between executions
						false                     // True to loop continuously, False to run only once
					);
				}
				
				return;
			}
		}
	}
}

void UAC_ShootingComponent::RestartShoot()
{
	CanShoot = 0;
}
