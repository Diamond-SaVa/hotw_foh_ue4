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
	
	UE_LOG(LogTemp, Warning,
		TEXT("Owner: %s | Component: %p | Classes: %d"),
		*GetNameSafe(GetOwner()),
		this,
		ClassProjectileByMaskArray.Num()
	);
	
	if (ClassProjectileByMaskArray.IsValidIndex(0))
	{
		LoadedProjectileClass = ClassProjectileByMaskArray[0].LoadSynchronous();

		UE_LOG(LogTemp, Warning,
			TEXT("[%s] Loaded Projectile Class: %s"),
			*GetNameSafe(GetOwner()),
			*GetNameSafe(LoadedProjectileClass.Get()));
	}
	
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
	if (IsValid(World) == false || bCanShoot == false)
	{
		return;
	}
	
	SpawnNextProjectileIfNeeded(World, AttackValue);
}

void UAC_ShootingComponent::SpawnNextProjectileIfNeeded(UWorld* World, int32 AttackValue)
{
	// Set the amount of projectiles in the array
	const int32 ArrayItemCount = ProjectileArray.Num();

	// If there are projectiles in the array,
	if (ArrayItemCount > 0)
	{
		int32 Count = 0;
		// get each AActor in the array.
		for (AActor* ProjectileActor : ProjectileArray)
		{
			if (Count >= MaxShots)
			{
				return;
			}
			
			// Check if it uses the IProjectiles Interface 
			if (IProjectiles* ProjectileInterface = Cast<IProjectiles>(ProjectileActor))
			{
				// Check the EMaskState value of the projectile
				const EMaskState ProjectileMaskType = ProjectileInterface->GetProjectileForMaskType();
				
				// Compare it with the Mask we have equipped
				if (ProjectileMaskType == CurrentMaskFilter)
				{
					if (ProjectileInterface->GetProjectileIsReady() == true)
					{
						// Shoot the returned projectile and end the function and for-loop
						ShootNextAvailableProjectile(World, ProjectileActor, AttackValue);
						return;
					}
					
					++Count;
				}
			}
		}
	};
	
	// In case of class's invalidity, stop the function from spawning an invalid object
	if (IsValid(LoadedProjectileClass) == false)
	{
		return;
	}
	
	// Otherwise, if the array doesn't feature any projectiles in the array, or
	if (AActor* ANewProjectile = World->SpawnActor(LoadedProjectileClass))
	{
		if (AA_Projectile* ProjectileData = Cast<AA_Projectile>(LoadedProjectileClass.Get()))
		{
			UE_LOG(LogTemp, Warning, TEXT("ProjectileData read from LoadedProjectileClass.Get()"));
			
			const EMaskState ProjectileMaskState = ProjectileData->GetProjectileForMaskType();
		
			if (CurrentMaskFilter != ProjectileMaskState)
			{
				return;
			}
		}
		
		ProjectileArray.Add(ANewProjectile);
		ShootNextAvailableProjectile(World, ANewProjectile, AttackValue);
	}
}

void UAC_ShootingComponent::ShootNextAvailableProjectile(const UWorld* World, AActor* NextProjectile, int32 AttackValue)
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

void UAC_ShootingComponent::SetMaskAndProjectile(EMaskState NewMaskState)
{
	CurrentMaskFilter = NewMaskState;
	
	const uint8 MaskIndex = static_cast<uint8>(NewMaskState);
	
	if (ClassProjectileByMaskArray.IsValidIndex(MaskIndex))
	{
		LoadedProjectileClass = ClassProjectileByMaskArray[MaskIndex].LoadSynchronous();

		UE_LOG(LogTemp, Warning,
			TEXT("[%s] Loaded Projectile Class: %s"),
			*GetNameSafe(GetOwner()),
			*GetNameSafe(LoadedProjectileClass.Get()));
	}
}
