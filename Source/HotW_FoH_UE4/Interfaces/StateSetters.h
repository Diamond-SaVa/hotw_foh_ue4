// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HotW_FoH_UE4/Enums/ECharacterEnums.h"
#include "StateSetters.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UStateSetters : public UInterface
{
	GENERATED_BODY()
};

/**
 * Provides functions for performative AnimBP state switching and calling specific and reusable functions
 */
class HOTW_FOH_UE4_API IStateSetters
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, Category = "Anim Setters")
	virtual void SetMovementState(const EMovementState NewState) = 0;
	
	UFUNCTION(BlueprintCallable, Category = "Anim Setters")
	virtual void SetActionState(const EActionState NewState) = 0;
	
	UFUNCTION(BlueprintCallable, Category = "Anim Setters")
	virtual void SetSpeedState(const ESpeedState NewState) = 0;
	
	UFUNCTION(BlueprintCallable, Category = "Anim Setters")
	virtual void SetBoolGoingUp(const bool IsGoingUp) = 0;
};
