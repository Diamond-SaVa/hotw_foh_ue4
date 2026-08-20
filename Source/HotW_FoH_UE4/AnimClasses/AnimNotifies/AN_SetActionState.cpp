// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "AN_SetActionState.h"

#include "HotW_FoH_UE4/Interfaces/StateSetters.h"

void UAN_SetActionState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	if (IsValid(MeshComp) == true)
	{
		AActor* Owner = MeshComp->GetOwner();
		
		if (IsValid(Owner) == true)
		{
			if (IStateSetters* StateInterface = Cast<IStateSetters>(Owner))
			{
				StateInterface->SetActionState(ActionState);
			}
		}
	}
}
