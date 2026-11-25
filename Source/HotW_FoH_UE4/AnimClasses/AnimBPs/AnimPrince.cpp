// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "AnimPrince.h"

#include "HotW_FoH_UE4/Characters/PC_C_Prince.h"

void UAnimPrince::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAnimPrince::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (LerpValue < 1.0f)
	{
		LerpValue = FMath::Clamp(LerpValue + DeltaSeconds, 0.0f, 1.0f);
		
		WalkSpeedLerp = FMath::Lerp(WalkSpeedLerp, SpeedGoal, LerpValue);
	}
}

void UAnimPrince::SetMovementState(const EMovementState NewState)
{
	AnimMovementState = NewState;
}

void UAnimPrince::SetActionState(const EActionState NewState)
{
	AnimActionState = NewState;
}

void UAnimPrince::SetSpeedState(const ESpeedState NewState)
{
	if (AnimSpeedState != NewState)
	{
		AnimSpeedState = NewState;
		
		LerpValue = 0.0f;
		
		switch (AnimSpeedState)
		{
		case ESpeedState::ESP_Stop:
			SpeedGoal = 0.0f;
			break;
		case ESpeedState::ESP_Walk:
			SpeedGoal = 0.5f;
			break;
		case ESpeedState::ESP_Run:
			SpeedGoal = 1.0f;
			break;
		}
	}
}

void UAnimPrince::SetBoolGoingUp(const bool IsGoingUp)
{
	bAnimGoingUp = IsGoingUp;
}
