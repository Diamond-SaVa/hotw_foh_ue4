// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "A_Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "HotW_FoH_UE4/Interfaces/Damageable.h"


// Sets default values
AA_Projectile::AA_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("TheProjectileMovement"));
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TheStaticMesh"));
	
	SetRootComponent(StaticMesh);
}

// Called when the game starts or when spawned
void AA_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
	StaticMesh->SetVisibility(false);
	
	ProjectileMovement->Deactivate();
	
	ProjectileIsReady = true;
	
	SetActorTickEnabled(false);
}

void AA_Projectile::OnProjectileSweepOverlaps(AActor* ActorHit)
{
	// Use function to override confirmed Sweep behaviors on children
}

// Called every frame
void AA_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/**/
	UWorld* World = GetWorld();
	if (World)
	{
		FHitResult Hit;
		
		FVector SweepLocation = GetActorLocation();
		
		constexpr ECollisionChannel PawnHitChannel = ECollisionChannel::ECC_Visibility;
		
		FQuat Rotator = FQuat::Identity;
		
		FCollisionShape HitShape = FCollisionShape::MakeSphere(32.0f);
		
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		if (ProjectileOwner.IsValid())
		{
			CollisionParams.AddIgnoredActor(ProjectileOwner.Get());
		}
		
		const bool bHitPawn = World->SweepSingleByChannel(Hit, SweepLocation, SweepLocation,
			Rotator, PawnHitChannel, HitShape, CollisionParams);
		
		if (bHitPawn == true)
		{
			if (AActor* ActorHit = Hit.GetActor())
			{
				OnProjectileSweepOverlaps(ActorHit);
			}
		}
	}
	/**/
}

void AA_Projectile::ActivateProjectile(AActor* ActorSource, const float MovementSpeedChange,
	const float NewAttackValue, const float GravityChange, const float LifeTime)
{
	if (ActorSource == nullptr || ProjectileMovement == nullptr)
	{
		return;
	}
	
	if (ProjectileOwner != ActorSource)
	{
		ProjectileOwner = ActorSource;
	}
	
	if (AttackValue != NewAttackValue)
	{
		AttackValue = NewAttackValue;
	}
	
	StaticMesh->SetVisibility(true);
	
	FVector ActorLocation = ActorSource->GetActorLocation();
	const float ActorFwdDirection = FMath::RoundFromZero(ActorSource->GetActorForwardVector().X);
	ActorLocation.X += ActorFwdDirection * 100.0f;
	SetActorLocation(ActorLocation);
	
	ProjectileMovement->Activate(true);
	ProjectileMovement->Velocity.X = ActorFwdDirection * MovementSpeedChange;
	ProjectileMovement->ProjectileGravityScale = GravityChange;
	
	ProjectileIsReady = false;
	
	SetActorTickEnabled(true);
	
	FTimerManager& WorldTimer = GetWorldTimerManager();
					
	WorldTimer.SetTimer(
		LifeTimerHandle,              // The tracking handle
		this,                     // The context object running the function
		&AA_Projectile::DeactivateProjectile, // The address of your function
		LifeTime,                     // Time in seconds between executions
		false                     // True to loop continuously, False to run only once
	);
}

void AA_Projectile::DeactivateProjectile()
{
	if (ProjectileMovement == nullptr)
	{
		return;
	}
	
	FTimerManager& WorldTimer = GetWorldTimerManager();
					
	if (WorldTimer.IsTimerActive(LifeTimerHandle) == true)
	{
		WorldTimer.ClearTimer(LifeTimerHandle);
	}
	
	StaticMesh->SetVisibility(false);
	
	ProjectileMovement->Deactivate();
	
	ProjectileIsReady = true;
	
	SetActorTickEnabled(false);
}