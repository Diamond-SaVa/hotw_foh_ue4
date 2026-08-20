// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_AtkTrace.generated.h"

/**
 * 
 */
UCLASS()
class HOTW_FOH_UE4_API UAN_AtkTrace : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="Asset Data Variables")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	
	UPROPERTY(EditAnywhere, Category="Asset Data Variables")
	float TraceDistance_X;
	
	UPROPERTY(EditAnywhere, Category="Asset Data Variables")
	float TraceDistance_Z;
	
	UPROPERTY(EditAnywhere, Category="Asset Data Variables")
	float TraceRadius = 20.0f;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
