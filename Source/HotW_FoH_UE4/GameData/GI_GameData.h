// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Datasmith/DatasmithCore/Public/DatasmithDefinitions.h"
#include "Engine/GameInstance.h"
#include "GI_GameData.generated.h"

/**
 * 
 */
UCLASS()
class HOTW_FOH_UE4_API UGI_GameData : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	// Current amount of money currently held by player
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadWrite)
	int32 Money = 0;
	
	// Base HP chosen by the player for difficulty modifiers
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadWrite)
	uint8 BaseHP = 4;
	
	// Base EP chosen by the player for difficulty modifiers
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadWrite)
	uint8 BaseEP = 4;
	
	// Current Level of the Player Characters
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadWrite)
	uint8 PlayerLevel = 1;
	
	// Current Level spent on Health Upgrades
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadWrite)
	uint8 HealthLevel = 1;
	
	// Current Level spent on Energy Upgrades
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadWrite)
	uint8 EnergyLevel = 1;
	
	// Current Level spent on Attack Upgrades
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadWrite)
	uint8 AttackLevel = 1;
	
public:
	// Get Base HP
	FORCEINLINE uint8 GetBaseHP() const { return BaseHP; }
	
	// Set Base HP
	FORCEINLINE void SetBaseHP(uint8 Value) { BaseHP = Value; }
	
	// Get Base EP
	FORCEINLINE uint8 GetBaseEP() const { return BaseEP; }
	
	// Set Base EP
	FORCEINLINE void SetBaseEP(uint8 Value) { BaseEP = Value; }
	
	// Get Player Level
	FORCEINLINE uint8 GetPlayerLevel() const { return PlayerLevel; }
	
	// Set Player Level
	FORCEINLINE void SetPlayerLevel(uint8 Value) { PlayerLevel = Value; }
	
	// Get Health Level
	FORCEINLINE uint8 GetHealthLevel() const { return HealthLevel; }
	
	// Set Health Level
	FORCEINLINE void SetHealthLevel(uint8 Value) { HealthLevel = Value; }
	
	// Get Energy Level
	FORCEINLINE uint8 GetEnergyLevel() const { return EnergyLevel; }
	
	// Set Energy Level
	FORCEINLINE void SetEnergyLevel(uint8 Value) { EnergyLevel = Value; }
	
	// Get Attack Level
	FORCEINLINE uint8 GetAttackLevel() const { return AttackLevel; }
	
	// Set Attack Level
	FORCEINLINE void SetAttackLevel(uint8 Value) { AttackLevel = Value; }
	
	// Get Money
	FORCEINLINE int32 GetMoney() const { return Money; }
	
	// Set Money
	FORCEINLINE void SetMoney(int32 Value) { Money = Value; }
	
	// Add Money
	FORCEINLINE void AddMoney(int32 Value) { Money = FMath::Clamp(Money + Value, 0, 9999); }
	
	// Remove Money
	FORCEINLINE void RemoveMoney(int32 Value) { Money = FMath::Clamp(Money - Value, 0, 9999); }
};
