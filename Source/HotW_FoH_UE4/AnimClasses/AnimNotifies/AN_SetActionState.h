// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "HotW_FoH_UE4/Enums/ECharacterEnums.h"
#include "AN_SetActionState.generated.h"

/**
 * 
 */
UCLASS()
class HOTW_FOH_UE4_API UAN_SetActionState : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="Variables")
	EActionState ActionState;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
