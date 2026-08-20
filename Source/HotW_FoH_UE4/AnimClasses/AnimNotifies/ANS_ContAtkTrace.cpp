// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "ANS_ContAtkTrace.h"

#include "HotW_FoH_UE4/Interfaces/Damageable.h"

void UANS_ContAtkTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	if (IsValid(MeshComp) == true)
	{
		OwnerActor = MeshComp->GetOwner();
	}
}

void UANS_ContAtkTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	
	if (OwnerActor.IsValid() == false)
	{
		if (IsValid(MeshComp) == true)
		{
			OwnerActor = MeshComp->GetOwner();
		}
		else
		{
			return;
		}
	}
	
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
