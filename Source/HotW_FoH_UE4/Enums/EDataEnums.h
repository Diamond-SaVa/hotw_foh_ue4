// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "EDataEnums.generated.h"

/**
 * List of Levels to identify each Level in the game
 */
UENUM(BlueprintType)
enum class ELevel : uint8
{
	ELV_Capital = 0 UMETA(DisplayName = "The Capital"),
	ELV_FinalTower = 99 UMETA(DisplayName = "The Final Tower of Time")
};

UENUM(BlueprintType)
enum class EStat : uint8
{
	EST_Health = 0 UMETA(DisplayName = "Health Stat"),
	EST_Energy = 1 UMETA(DisplayName = "Energy Stat"),
	EST_Attack = 2 UMETA(DisplayName = "Attack Stat")
};
