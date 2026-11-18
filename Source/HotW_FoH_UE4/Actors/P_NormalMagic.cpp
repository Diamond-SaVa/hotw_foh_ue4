// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "P_NormalMagic.h"

#include "HotW_FoH_UE4/Interfaces/Damageable.h"


// Sets default values
AP_NormalMagic::AP_NormalMagic()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileForMask = EMaskState::EM_None;
}

// Called when the game starts or when spawned
void AP_NormalMagic::BeginPlay()
{
	Super::BeginPlay();
	
}

void AP_NormalMagic::OnProjectileSweepOverlaps(AActor* ActorHit)
{
	Super::OnProjectileSweepOverlaps(ActorHit);
	
	if (IDamageable* Damageable = Cast<IDamageable>(ActorHit))
	{
		Damageable->TakeDamage(AttackValue);
	}
	
	DeactivateProjectile();
}

// Called every frame
void AP_NormalMagic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

