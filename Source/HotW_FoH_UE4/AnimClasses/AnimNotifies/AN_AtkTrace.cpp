// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "AN_AtkTrace.h"

#include "HotW_FoH_UE4/Interfaces/Damageable.h"

void UAN_AtkTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	// Proceed if the MeshComponent is valid,
	if (IsValid(MeshComp) == false)
	{
		return;
	}
	
	// if the MeshComp's Owner is also valid,
	AActor* OwnerActor = MeshComp->GetOwner();
	if (IsValid(OwnerActor) == false)
	{
		return;
	}
	
	// Then make sure the owner has the Damageable Interface
	if (IDamageable* DamageInterface = Cast<IDamageable>(OwnerActor))
	{
		FVector TraceLocation = OwnerActor->GetActorLocation();
		
		FVector PointDir = OwnerActor->GetActorForwardVector();
		PointDir.X *= TraceDistance_X;
		PointDir.Z += TraceDistance_Z;
		
		TraceLocation += PointDir;
		
		DamageInterface->SphereTraceDamage(TraceLocation, TraceLocation, 
			ObjectTypes, TraceRadius);	
	}
}
