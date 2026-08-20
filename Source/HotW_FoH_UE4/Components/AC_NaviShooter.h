// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "AC_NaviShooter.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HOTW_FOH_UE4_API UAC_NaviShooter : public UPrimitiveComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	AActor* CachedOwnerActor;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component Data")
	UStaticMeshComponent* CachedMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location Modifier Data")
	UCurveFloat* CurveOrbitX;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location Modifier Data")
	UCurveFloat* CurveOrbitY;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location Modifier Data")
	UCurveFloat* CurveOrbitZ;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location Modifier Data")
	float RadiusDistance = 150.0;
	
protected:
	float TLerp = 0.0f;

public:
	// Sets default values for this component's properties
	UAC_NaviShooter();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
