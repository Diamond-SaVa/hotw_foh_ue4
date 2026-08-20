// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "SprArm_PC_CamBoom.h"


// Sets default values for this component's properties
USprArm_PC_CamBoom::USprArm_PC_CamBoom()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// CRITICAL: Ensures camera math happens AFTER character movement/physics calculation
	PrimaryComponentTick.TickGroup = TG_PostPhysics; 
}


// Called when the game starts
void USprArm_PC_CamBoom::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USprArm_PC_CamBoom::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	const AActor* Owner = GetOwner();
	if (!IsValid(Owner) || Owner->IsPendingKill())
	{
		return;
	}
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (FMath::IsNearlyEqual(SocketOffset.Y, TargetOffset_Y, 0.1f) == false)
	{
		SocketOffset.Y = FMath::FInterpTo(SocketOffset.Y, TargetOffset_Y, DeltaTime, OffsetSpeed);
	}
}

