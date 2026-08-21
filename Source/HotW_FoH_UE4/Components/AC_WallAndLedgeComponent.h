// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_WallAndLedgeComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HOTW_FOH_UE4_API UAC_WallAndLedgeComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	class APC_C_Prince* PCOwner;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Assets")
	UAnimMontage* AM_WallStart;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Assets")
	UAnimMontage* AM_LedgeStart;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation Assets")
	UAnimMontage* AM_LedgeAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wall Trace Modifiers")
	float WallTraceHeightModifier = 0.75f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wall Trace Modifiers")
	float WallHalfHeightMod;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wall Trace Modifiers")
	float WallRadiusMod;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wall Trace Modifiers")
	int32 IsDetectingWallFromTop;
	

public:
	// Sets default values for this component's properties
	UAC_WallAndLedgeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	void WallDetection();
	
	void LedgeDetection();
	
	void ResetOnLanded();
	
	void LedgeUpAnim();
	
	void LedgeDownAnim();
};
