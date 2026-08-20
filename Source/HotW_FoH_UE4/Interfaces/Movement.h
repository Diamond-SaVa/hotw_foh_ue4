// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "HotW_FoH_UE4/Enums/EDirectionEnums.h"
#include "UObject/Interface.h"
#include "Movement.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UMovement : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface Functions for Movement behavior
 */
class HOTW_FOH_UE4_API IMovement
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void DodgeInterface(const float DodgePower, const float DeltaSeconds, const EDirection DirectionEnum) = 0;
	
	virtual void LedgeJumpInterface() = 0;
	
	virtual void MoveInputInterface(const float Direction) = 0;
	
	virtual void CharacterVelocityInterface(const FVector& NewVelocity) = 0;
	
	virtual void CharacterVelocityInterface(const EDirection AxisDirection, const float NewVelocity = 0.0f) = 0;
};
