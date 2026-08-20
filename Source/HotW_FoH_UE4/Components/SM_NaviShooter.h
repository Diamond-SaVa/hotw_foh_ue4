// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "SM_NaviShooter.generated.h"

/**
 * 
 */
UCLASS()
class HOTW_FOH_UE4_API USM_NaviShooter : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	USM_NaviShooter();
	
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	UPROPERTY()
	AActor* CachedOwner;
	
	float TLerp = 0.0f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location Modifier Data")
	UCurveFloat* CurveOrbitX;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location Modifier Data")
	UCurveFloat* CurveOrbitY;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location Modifier Data")
	UCurveFloat* CurveOrbitZ;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location Modifier Data")
	float RadiusDistance = 150.0;
	
	FORCEINLINE AActor* GetCachedOwner() const { return CachedOwner; }
};
