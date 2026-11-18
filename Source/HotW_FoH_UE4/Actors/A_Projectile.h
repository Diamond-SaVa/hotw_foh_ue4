// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HotW_FoH_UE4/Enums/ECharacterEnums.h"
#include "HotW_FoH_UE4/Interfaces/Projectiles.h"
#include "A_Projectile.generated.h"

UCLASS()
class HOTW_FOH_UE4_API AA_Projectile : public AActor, public IProjectiles
{
	GENERATED_BODY()

protected:
	
	TWeakObjectPtr<AActor> ProjectileOwner;
	
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;
	
	FTimerHandle LifeTimerHandle;
	
	UPROPERTY(EditAnywhere, Category="Sweep Data")
	float SweepRadius = 32.0f;

	// 0 if active, 1 if inactive
	bool ProjectileIsReady = false;
	
	uint8 AttackValue = 1;
	
	EMaskState ProjectileForMask;
	
public:
	// Sets default values for this actor's properties
	AA_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnProjectileSweepOverlaps(AActor* ActorHit);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void DeactivateProjectile();
	
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	
	FORCEINLINE UStaticMeshComponent* GetStaticMesh() const { return StaticMesh; }
	
	/* IProjectiles Virtual Interface Functions */
	
	virtual void ActivateProjectile(AActor* ActorSource, float MovementSpeedChange, float NewAttackValue,
									float GravityChange, float LifeTime) override;
	
	FORCEINLINE virtual EMaskState GetProjectileForMaskType() override { return ProjectileForMask; }
	
	FORCEINLINE virtual bool GetProjectileIsReady() override { return ProjectileIsReady; }
	
	/* Getters and Setters */
	
	FORCEINLINE void SetProjectileIsActive(const bool IsActive) { ProjectileIsReady = IsActive; }
};
