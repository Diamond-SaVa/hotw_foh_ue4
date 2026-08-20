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
	
	if (ProjectileMovement != nullptr)
	{
		ProjectileMovement->Deactivate();
	}
}

// Called every frame
void AA_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UWorld* World = GetWorld();
	if (ProjectileIsActive == 1 && World)
	{
		FHitResult Hit;
		
		FVector SweepLocation = GetActorLocation();
		
		constexpr ECollisionChannel PawnHitChannel = ECollisionChannel::ECC_Visibility;
		
		FCollisionShape HitShape = FCollisionShape::MakeSphere(32.0f);
		
		FCollisionQueryParams CollisionParams;
		// CollisionParams.AddIgnoredActor(this);
		
		/**/
		const bool bHitPawn = World->SweepSingleByChannel(Hit, SweepLocation, SweepLocation,
			FQuat::Identity, PawnHitChannel, HitShape, CollisionParams);
		
		if (bHitPawn == true)
		{
			if (IsValid(Hit.GetActor()) == true)
			{
				if (IDamageable* Damageable = Cast<IDamageable>(Hit.GetActor()))
				{
					FTimerManager& WorldTimer = GetWorldTimerManager();
					
					if (WorldTimer.IsTimerActive(TimerHandle))
					{
						WorldTimer.ClearTimer(TimerHandle);
					}
			
					DeactivateProjectile();
			
					return;
				}
				
				FString Debug = FString::Printf(TEXT("IMPACT : %f, %f, %f"), Hit.ImpactNormal.X, 
					Hit.ImpactNormal.Y, Hit.ImpactNormal.Z);
				
				GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, 
						Debug);
				
				
				if (FMath::IsNearlyZero(Hit.ImpactNormal.Z, 0.01f) == false)
				{
					if (Hit.ImpactNormal.Z > 0.0)
					{
						ProjectileMovement->Velocity.Z = BounceSpeed;
						return;
					}
				
					if (Hit.ImpactNormal.Z < 0.0)
					{
						ProjectileMovement->Velocity.Z = -BounceSpeed;
						return;
					}
				}
			
				if (FMath::IsNearlyZero(Hit.ImpactNormal.X, 0.01f) == false)
				{
					if (Hit.ImpactNormal.X != 0.0)
					{
						ProjectileMovement->Velocity.X *= -1.0f;
						return;
					}
				}
			}
		}
	}
}

void AA_Projectile::ActivateProjectile(AActor* ActorSource, const float MovementSpeedChange,
	const float BounceSpeedChange, const float GravityChange)
{
	if (ActorSource == nullptr || ProjectileMovement == nullptr)
	{
		return;
	}
	
	StaticMesh->SetVisibility(true);
	
	FVector ActorLocation = ActorSource->GetActorLocation();
	const float ActorFwdDirection = ActorSource->GetActorForwardVector().X;
	ActorLocation.X += ActorFwdDirection * 100.0f;
	SetActorLocation(ActorLocation);
	
	ProjectileMovement->Activate(true);
	ProjectileMovement->Velocity.X = ActorFwdDirection * MovementSpeedChange;
	ProjectileMovement->ProjectileGravityScale = GravityChange;
	ProjectileMovement->Velocity.Z = -BounceSpeed;
	
	ProjectileIsActive = 1;
	
	FTimerManager& WorldTimer = GetWorldTimerManager();
					
	WorldTimer.SetTimer(
		TimerHandle,              // The tracking handle
		this,                     // The context object running the function
		&AA_Projectile::DeactivateProjectile, // The address of your function
		3.0f,                     // Time in seconds between executions
		false                     // True to loop continuously, False to run only once
	);
}

void AA_Projectile::DeactivateProjectile()
{
	if (ProjectileMovement == nullptr)
	{
		return;
	}
	
	StaticMesh->SetVisibility(false);
	
	ProjectileMovement->Deactivate();
	
	ProjectileIsActive = 0;
}

