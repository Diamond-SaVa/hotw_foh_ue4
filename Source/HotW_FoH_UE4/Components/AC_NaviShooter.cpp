// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "AC_NaviShooter.h"


// Sets default values for this component's properties
UAC_NaviShooter::UAC_NaviShooter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	
	if (CachedMeshComponent == nullptr)
	{
		CachedMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	}
}


// Called when the game starts
void UAC_NaviShooter::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	// Get the Owner of the Component
	CachedOwnerActor = GetOwner();
	if (CachedOwnerActor && CachedOwnerActor->GetRootComponent() && IsValid(CachedMeshComponent))
	{
		// Attach this component to the owner's root
		CachedMeshComponent->AttachToComponent(CachedOwnerActor->GetRootComponent(), 
			FAttachmentTransformRules::KeepRelativeTransform);
	}
}


// Called every frame
void UAC_NaviShooter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (CachedMeshComponent == nullptr || CachedOwnerActor == nullptr)
	{
		return;
	}
	
	FVector NewLocation = CachedOwnerActor->GetActorLocation();
	
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
	
	CachedMeshComponent->SetWorldLocation(NewLocation);
}

