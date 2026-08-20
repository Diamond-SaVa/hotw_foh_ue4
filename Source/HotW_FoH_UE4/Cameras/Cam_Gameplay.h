// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "Cam_Gameplay.generated.h"

class APC_C_Prince;

UCLASS()
class HOTW_FOH_UE4_API ACam_Gameplay : public ACameraActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cam_Gameplay")
	TWeakObjectPtr<AActor> TargetActor;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cam_Gameplay")
	float OffsetFromTargetX = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cam_Gameplay")
	float OffsetFromTargetY = 750.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cam_Gameplay")
	float OffsetFromTargetZ = 200.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cam_Gameplay")
	float CameraSpeedX = 2.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cam_Gameplay")
	float CameraSpeedZ = 5.0f;

public:
	// Sets default values for this actor's properties
	ACam_Gameplay();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void SetTargetActor(AActor* NewTargetActor);
};
