// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "AC_MaskComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "HotW_FoH_UE4/Characters/PC_C_Prince.h"


// Sets default values for this component's properties
UAC_MaskComponent::UAC_MaskComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UAC_MaskComponent::SwitchMask(EMaskState NewMaskState)
{
	MaskState = NewMaskState;

	UpdateMaskOnCharacter();
}

void UAC_MaskComponent::UpdateMaskOnCharacter()
{
	if (APC_C_Prince* OwnerPC = Cast<APC_C_Prince>(GetOwner()))
	{
		UStaticMeshComponent* MaskComponent = OwnerPC->GetMaskStaticMesh();
		
		if (!IsValid(MaskComponent))
		{
			UE_LOG(LogTemp, Warning,
				TEXT("Mask component is invalid!"));
			return;
		}
	
		if (UWorld* World = GetWorld())
		{
			if (NS_MaskEffect != nullptr)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					World,
					NS_MaskEffect,
					MaskComponent->GetComponentLocation()
				);
			}
		}
		
		switch (MaskState)
		{
		case EMaskState::EM_None:
			// OwnerPC set Mask Static Mesh invisible
			OwnerPC->GetMaskStaticMesh()->SetVisibility(false);
			
			break;
		default:
			// Load Mask Static Mesh, THEN OwnerPC set Mask Static Mesh visible AND swap reference to loaded Mask
			if (MaskMeshArray.Num() > 0)
			{
				const int32 EnumValue = static_cast<int32>(MaskState) - 1;

				if (!MaskMeshArray.IsValidIndex(EnumValue))
				{
					UE_LOG(LogTemp, Warning,
						TEXT("Invalid mask index: %d"), EnumValue);
					return;
				}
				
				UStaticMesh* MaskToLoad = MaskMeshArray[EnumValue].LoadSynchronous();
			
				if (!IsValid(MaskToLoad))
				{
					UE_LOG(LogTemp, Warning,
						TEXT("Failed to load mask at index %d"), EnumValue);
					return;
				}
				
				// Assign the loaded mesh to the component
				MaskComponent->SetStaticMesh(MaskToLoad);

				// Make the component visible
				MaskComponent->SetVisibility(true);
			}
			
			break;	
		}	
	}
}


// Called when the game starts
void UAC_MaskComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

