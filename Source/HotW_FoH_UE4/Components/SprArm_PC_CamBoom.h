// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "SprArm_PC_CamBoom.generated.h"

/*
 * Custom-made Spring Arm Component that updates itself when the character moves, looks up or down.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HOTW_FOH_UE4_API USprArm_PC_CamBoom : public USpringArmComponent
{
	GENERATED_BODY()
	
protected:
	// Camera Side Direction
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera Data")
	float TargetOffset_Y = 110.0f;;
	
	// Camera shift speed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera Data")
	float OffsetSpeed = 2.0f;;

public:
	// Sets default values for this component's properties
	USprArm_PC_CamBoom();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	FORCEINLINE void SetTargetOffset_Y(const float NewTargetOffset_Y)
	{
		if (TargetOffset_Y != NewTargetOffset_Y)
		{
			TargetOffset_Y = NewTargetOffset_Y;
		}
	}
};
