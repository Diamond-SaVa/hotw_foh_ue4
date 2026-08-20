// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HotW_FoH_UE4/Enums/EDataEnums.h"
#include "SG_SaveData.generated.h"

/**
 * Save Data to store and retreive for future reference
 */
UCLASS()
class HOTW_FOH_UE4_API USG_SaveData : public USaveGame
{
	GENERATED_BODY()
	
public:
	// Location where player last saved game
	UPROPERTY(Category = "Game Data", VisibleAnywhere, BlueprintReadWrite)
	FVector LastLocationSaved;
	
	// Current amount of money currently held by player
	UPROPERTY(Category = "PLayer Data", VisibleAnywhere, BlueprintReadWrite)
	int32 Money;
	
	// Base HP chosen by the player for difficulty modifiers
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadWrite)
	uint8 BaseHP;
	
	// Base EP chosen by the player for difficulty modifiers
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadWrite)
	uint8 BaseEP;
	
	// Current Level of the Player Characters
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadWrite)
	uint8 PlayerLevel;
	
	// Current Level spent on Health Upgrades
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadWrite)
	uint8 HealthLevel;
	
	// Current Level spent on Energy Upgrades
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadWrite)
	uint8 EnergyLevel;
	
	// Current Level spent on Attack Upgrades
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadWrite)
	uint8 AttackLevel;
	
	// Level where player last saved
	UPROPERTY(Category = "Game Data", VisibleAnywhere, BlueprintReadWrite)
	ELevel LevelLastSaved;
};
