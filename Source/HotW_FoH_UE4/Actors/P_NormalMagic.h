// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "A_Projectile.h"
#include "P_NormalMagic.generated.h"

UCLASS()
class HOTW_FOH_UE4_API AP_NormalMagic : public AA_Projectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AP_NormalMagic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnProjectileSweepOverlaps(AActor* ActorHit) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};