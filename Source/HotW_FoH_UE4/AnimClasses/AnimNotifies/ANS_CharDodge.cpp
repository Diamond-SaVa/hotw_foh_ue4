// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#include "ANS_CharDodge.h"

#include "HotW_FoH_UE4/Interfaces/Movement.h"

void UANS_CharDodge::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	// Checks the validity of the mesh component
	if (IsValid(MeshComp) == true)
	{
		// and sets its owner to the allocated slot.
		CachedOwner = MeshComp->GetOwner();
		
		const FString Name = CachedOwner->GetName() + " from Begin";
		
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, Name);
	}
}

void UANS_CharDodge::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	
	// Validates the CachedOwner, and if it's not, do not continue with this function's code
	if (CachedOwner.IsValid() == false)
	{
		// in the case that, for some reason, the Owner couldn't be cached, do so right now
		CachedOwner = MeshComp->GetOwner();

		const FString Name = CachedOwner->GetName() + " from Tick";
		
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Black, Name);
		return;
	}
	
	// Fast, native C++ interface cast to IMovement from the CachedOwner's pointer
	if (IMovement* MovementInterface = Cast<IMovement>(CachedOwner.Get()))
	{
		switch (DashDirection)
		{
		case EDirection::EDI_Backward:
		case EDirection::EDI_Forward:
			MovementInterface->DodgeInterface(DodgePower, FrameDeltaTime, DashDirection);
			break;
		case EDirection::EDI_X:
		case EDirection::EDI_Y:
		case EDirection::EDI_Z:
			MovementInterface->CharacterVelocityInterface(DashDirection, DodgePower);
			break;
		default:
			break;
		}
		
	}
}

void UANS_CharDodge::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}
