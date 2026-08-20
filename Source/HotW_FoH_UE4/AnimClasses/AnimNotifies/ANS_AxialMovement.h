// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "HotW_FoH_UE4/Enums/EDirectionEnums.h"
#include "ANS_AxialMovement.generated.h"

/**
 * AnimNotifyState that moves the animated character in a specific direction
 */
UCLASS()
class HOTW_FOH_UE4_API UANS_AxialMovement : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	// The owner of this instance of Anim Notify State
	TWeakObjectPtr<AActor> CachedOwner;
	
	// Changes the direction at which the character will dash 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Variables")
	EDirection MoveDirection;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
