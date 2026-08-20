// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_ContAtkTrace.generated.h"

/**
 * 
 */
UCLASS()
class HOTW_FOH_UE4_API UANS_ContAtkTrace : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	TWeakObjectPtr<AActor> OwnerActor;
	
	UPROPERTY(EditAnywhere, Category="Asset Data Variables")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	
	UPROPERTY(EditAnywhere, Category="Asset Data Variables")
	float TraceDistance_X;
	
	UPROPERTY(EditAnywhere, Category="Asset Data Variables")
	float TraceDistance_Z;
	
	UPROPERTY(EditAnywhere, Category="Asset Data Variables")
	float TraceRadius = 20.0f;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
};
