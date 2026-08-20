// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "AN_LedgeJumpCall.h"

#include "HotW_FoH_UE4/Interfaces/Movement.h"

void UAN_LedgeJumpCall::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	AActor* Owner = MeshComp->GetOwner();
	if (IsValid(Owner) == false)
	{
		return;
	}
	
	if (IMovement* MoveInterface = Cast<IMovement>(Owner))
	{
		MoveInterface->LedgeJumpInterface();
	}
}
