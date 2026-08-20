// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "EDirectionEnums.generated.h"

/**
 * List of Directions that can affect some behaviors
 */
UENUM(BlueprintType)
enum class EDirection : uint8
{
	EDI_Forward		= 0 UMETA(DisplayName = "Forward"),
	EDI_Backward	= 1 UMETA(DisplayName = "Backward"),
	EDI_Left		= 2 UMETA(DisplayName = "Left"),
	EDI_Right		= 3 UMETA(DisplayName = "Right"),
	EDI_Up			= 4 UMETA(DisplayName = "Up"),
	EDI_Down		= 5 UMETA(DisplayName = "Down"),
	EDI_X			= 6 UMETA(DisplayName = "Down"),
	EDI_Y			= 7 UMETA(DisplayName = "Down"),
	EDI_Z			= 8 UMETA(DisplayName = "Down"),
};
