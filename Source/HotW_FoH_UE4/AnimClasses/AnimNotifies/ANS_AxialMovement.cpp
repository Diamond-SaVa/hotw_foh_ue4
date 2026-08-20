// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "ANS_AxialMovement.h"

#include "HotW_FoH_UE4/Interfaces/Movement.h"

void UANS_AxialMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	if (IsValid(MeshComp) == true)
	{
		CachedOwner = MeshComp->GetOwner();
	}
}

void UANS_AxialMovement::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	
	if (CachedOwner.IsValid() == false)
	{
		CachedOwner = MeshComp->GetOwner();
		return;
	}
	
	if (IMovement* Movement = Cast<IMovement>(CachedOwner.Get()))
	{
		const FVector Forward = CachedOwner.Get()->GetActorForwardVector();
		const float Direction = FMath::CeilToFloat(Forward.X);
		Movement->MoveInputInterface(Direction);
	}
}

void UANS_AxialMovement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}
