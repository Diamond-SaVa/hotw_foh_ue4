// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_Projectile.generated.h"

UCLASS()
class HOTW_FOH_UE4_API AA_Projectile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;
	
	FTimerHandle TimerHandle;
	
	float BounceSpeed = 400.0;

public:
	// 0 if active, 1 if inactive
	int32 ProjectileIsActive = 0;
	
	// Sets default values for this actor's properties
	AA_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void ActivateProjectile(AActor* ActorSource, float MovementSpeedChange, float BounceSpeedChange,
		float GravityChange);
	
	virtual void DeactivateProjectile();
	
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	
	FORCEINLINE UStaticMeshComponent* GetStaticMesh() const { return StaticMesh; }
};
