// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "SM_NaviShooter.h"

USM_NaviShooter::USM_NaviShooter()
{
	PrimaryComponentTick.bCanEverTick = true;	
}

void USM_NaviShooter::BeginPlay()
{
	Super::BeginPlay();
	
	CachedOwner = GetOwner();
}

void USM_NaviShooter::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (CachedOwner == nullptr)
	{
		return;
	}
	
	FVector NewLocation = CachedOwner->GetActorLocation();
	
	if (CurveOrbitX != nullptr)
	{
		NewLocation.X += CurveOrbitX->GetFloatValue(TLerp);
	}
	
	if (CurveOrbitY != nullptr)
	{
		NewLocation.Y += CurveOrbitY->GetFloatValue(TLerp);
	}
	
	if (CurveOrbitZ != nullptr)
	{
		NewLocation.Z += CurveOrbitZ->GetFloatValue(TLerp);
	}
	
	TLerp += DeltaTime;
	
	if (TLerp > 3.0f)
	{
		TLerp = 0.0f;
	}
	
	SetWorldLocation(NewLocation);
}
