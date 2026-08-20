// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "AN_DestroyActorFromNotify.h"

void UAN_DestroyActorFromNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	if (IsValid(MeshComp) == true)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			Owner->Destroy();
		}
	}
}
