// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "AnimEnemy.h"

void UAnimEnemy::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAnimEnemy::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	/** /
	if (LerpValue < 1.0f)
	{
		LerpValue = FMath::Clamp(LerpValue + DeltaSeconds, 0.0f, 1.0f);
		
		WalkSpeedLerp = FMath::Lerp(WalkSpeedLerp, SpeedGoal, LerpValue);
	}
	/**/
}

void UAnimEnemy::SetMovementState(const EMovementState NewState)
{
	AnimMovementState = NewState;
}

void UAnimEnemy::SetActionState(const EActionState NewState)
{
	AnimActionState = NewState;
}

void UAnimEnemy::SetSpeedState(const ESpeedState NewState)
{
	
}

void UAnimEnemy::SetBoolGoingUp(const bool IsGoingUp)
{
	
}
