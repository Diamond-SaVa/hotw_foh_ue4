// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_LedgeJumpCall.generated.h"

/**
 * AnimNotify used to call the LedgeJumpInterface function for the owner of the animation.
 */
UCLASS()
class HOTW_FOH_UE4_API UAN_LedgeJumpCall : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
