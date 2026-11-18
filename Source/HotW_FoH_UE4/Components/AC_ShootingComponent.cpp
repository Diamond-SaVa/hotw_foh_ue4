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

void UAC_ShootingComponent::ShootAction(int32 AttackValue)
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false || GetCanShoot() == false)
	{
		return;
	}
	
	SpawnNextProjectileIfNeeded(World, AttackValue);
}

void UAC_ShootingComponent::SpawnNextProjectileIfNeeded(UWorld* World, int32 AttackValue)
{
	// Set the amount of projectiles in the array
	const int ArrayItemCount = ProjectileArray.Num();
	
	// If there are projectiles in the array,
	if (ArrayItemCount > 0)
	{
		// get each AActor in the array.
		for (AActor* ProjectileActor : ProjectileArray)
		{
			// Check if it uses the IProjectiles Interface 
			if (IProjectiles* ProjectileInterface = Cast<IProjectiles>(ProjectileActor))
			{
				// Check the EMaskState value of the projectile
				const EMaskState ProjectileMaskType = ProjectileInterface->GetProjectileForMaskType();
				
				// Compare it with the Mask we have equipped
				if (ProjectileMaskType == CurrentMaskFilter)
				{
					// And if the Projectile is active-
					if (ProjectileInterface->GetProjectileIsReady() == true)
					{
						// Shoot the returned projectile and end the function and for-loop
						ShootNextAvailableProjectile(World, ProjectileActor, AttackValue);
						return;
					}
				}
			}
		}
	}
	
	const uint8 MaskValue = static_cast<uint8>(CurrentMaskFilter);

	const TSoftClassPtr<AA_Projectile> ClassOfProjectile = ClassProjectileByMaskArray[MaskValue];
	
	// In case of class's invalidity, stop the function from spawning an invalid object
	if (ClassOfProjectile.IsValid() == false)
	{
		return;
	}
	
	// Otherwise, if the array doesn't feature any projectiles in the array, or
	if (AActor* ANewProjectile = World->SpawnActor(ClassOfProjectile.Get()))
	{
		ProjectileArray.Add(ANewProjectile);
		ShootNextAvailableProjectile(World, ANewProjectile, AttackValue);
	}
}

void UAC_ShootingComponent::ShootNextAvailableProjectile(UWorld* World, AActor* NextProjectile, int32 AttackValue)
{
	// Verify the NextProjectile is not nullptr
	if (NextProjectile == nullptr)
	{
		return;
	}
	
	if (IProjectiles* ProjectileInterface = Cast<IProjectiles>(NextProjectile))
	{
		const EMaskState ProjectileMask = ProjectileInterface->GetProjectileForMaskType();
		const bool bProjectileAvailable = ProjectileInterface->GetProjectileIsReady();
				
		if (CurrentMaskFilter == ProjectileMask && bProjectileAvailable == true)
		{
			ProjectileInterface->ActivateProjectile(GetOwner(), NextProjectileData.MovementSpeed, 
				AttackValue, NextProjectileData.GravityChange, NextProjectileData.LifeTime);
				
			bCanShoot = false;
				
			FTimerManager& WorldTimer = World->GetTimerManager();
					
			WorldTimer.SetTimer(
				CooldownTimerHandle,					// The tracking handle
				this,									// The context object running the function
				&UAC_ShootingComponent::RestartShoot,	// The address of your function
				0.5f,									// Time in seconds between executions
				false									// True to loop continuously, False to run only once
			);
		}
	}
}

void UAC_ShootingComponent::RestartShoot()
{
	bCanShoot = true;
}

void UAC_ShootingComponent::SetMaskState(const EMaskState NewState)
{
	CurrentMaskFilter = NewState;
}

void UAC_ShootingComponent::SetActionState(const EActionState NewState)
{
}

void UAC_ShootingComponent::SetBoolGoingUp(const bool IsGoingUp)
{
}

void UAC_ShootingComponent::SetMovementState(const EMovementState NewState)
{
}

void UAC_ShootingComponent::SetProjectileOfMask(const TSoftClassPtr<class AA_Projectile>& NewProjectile)
{
}

void UAC_ShootingComponent::SetSpeedState(const ESpeedState NewState)
{
}
