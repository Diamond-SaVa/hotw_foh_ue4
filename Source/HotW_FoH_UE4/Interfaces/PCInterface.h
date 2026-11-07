// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PCInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UPCInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HOTW_FOH_UE4_API IPCInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RegisterCharacterToControllerArray(class APC_C_Prince* PlayableCharacter, const int32 CharacterID) = 0;
	
	virtual void CheckIfCouldSwitchOnLanded() = 0;
};
