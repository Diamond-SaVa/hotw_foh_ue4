// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "HotW_FoH_UE4/Enums/EDirectionEnums.h"
#include "ANS_CharDodge.generated.h"

/**
 * Anim Notify State used to provide constant movement for dashes and dodges
 */
UCLASS()
class HOTW_FOH_UE4_API UANS_CharDodge : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	// The owner of this instance of Anim Notify State
	TWeakObjectPtr<AActor> CachedOwner;
	
	// Dodge Power amount in float
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Variables")
	float DodgePower = 500.0;
	
	// Changes the direction at which the character will dash 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Variables")
	EDirection DashDirection;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
