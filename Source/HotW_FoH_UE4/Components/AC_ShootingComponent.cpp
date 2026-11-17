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

void UAC_ShootingComponent::SpawnNextProjectile(UWorld* World)
{
	if (AActor* ANewProjectile = World->SpawnActor(ClassOfProjectile.Get()))
	{
		ProjectileArray.Add(ANewProjectile);
	}
}

void UAC_ShootingComponent::ShootNextAvailableProjectile()
{
	if (GetCanShoot() == false)
	{
		return;
	}
	
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		return;
	}
	
	if (ProjectileArray.Num() <= 0)
	{
		SpawnNextProjectile(World);
	}

	for (AActor* Projectile : ProjectileArray)
	{
		if (IsValid(Projectile) == false)
		{
			return;
		}
		
		if (AA_Projectile* Shoot = Cast<AA_Projectile>(Projectile))
		{
			
			if (Shoot->GetProjectileIsActive())
			{
				Shoot->ActivateProjectile(GetOwner(), NextProjectileData.MovementSpeed, 
				NextProjectileData.BounceSpeed, NextProjectileData.GravityChange, 3.0f);
				
				CanShoot = 1;
				
				FTimerManager& WorldTimer = World->GetTimerManager();
					
				WorldTimer.SetTimer(
					CooldownTimerHandle,              // The tracking handle
					this,                     // The context object running the function
					&UAC_ShootingComponent::RestartShoot, // The address of your function
					0.5f,                     // Time in seconds between executions
					false                     // True to loop continuously, False to run only once
				);
				
				return;
			}
			
			SpawnNextProjectile(World);
		}
	}
}

void UAC_ShootingComponent::RestartShoot()
{
	CanShoot = 0;
}
