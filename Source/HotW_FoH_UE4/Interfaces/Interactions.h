// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactions.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UInteractions : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HOTW_FOH_UE4_API IInteractions
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Interactions with NPCs, Triggers and others.
	UFUNCTION(BlueprintCallable, Category = "Damage Functions")
	virtual void InteractWith() = 0;
	
	// Opens Doors, Treasure Chests, among other openable objects.
	UFUNCTION(BlueprintCallable, Category = "Damage Functions")
	virtual void OpenTo() = 0;
};
