// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HotW_FoH_UE4/Interfaces/StateSetters.h"
#include "AnimEnemy.generated.h"

/**
 * 
 */
UCLASS()
class HOTW_FOH_UE4_API UAnimEnemy : public UAnimInstance, public IStateSetters
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	EMovementState AnimMovementState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	EActionState AnimActionState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	ESpeedState AnimSpeedState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	EHitState AnimHitState;
	
public:
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	virtual void SetMovementState(const EMovementState NewState) override;
	
	virtual void SetActionState(const EActionState NewState) override;
	
	virtual void SetSpeedState(const ESpeedState NewState) override;
	
	virtual void SetBoolGoingUp(const bool IsGoingUp) override;
};
