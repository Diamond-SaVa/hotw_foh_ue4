// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HotW_FoH_UE4/Interfaces/Manager.h"
#include "GM_Manager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterSwitch, AActor*, NewCharacter);

class USaveGame;
class ULevelStreamingDynamic;

/**
 * Game Mode class dedicated to managing and storing data
 */
UCLASS()
class HOTW_FOH_UE4_API AGM_Manager : public AGameModeBase, public IManager
{
	GENERATED_BODY()
	
public:
	// In the correct order, spawn: Prince, Girl, and then Camera
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actor Levels")
	TArray<TSoftClassPtr<AActor>> ArrayOfPreparedActors;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level Data")
	TSoftObjectPtr<UWorld> CurrentLevel;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level Data")
	TSoftObjectPtr<UWorld> CameraLevel;
	
	int32 CurrentCharacterSlot;

private:	
	UPROPERTY()
	ULevelStreamingDynamic* LoadedLevelInstance;
	
public:
	UPROPERTY(BlueprintAssignable, Category="Switch Events")
	FOnCharacterSwitch OnCharacterSwitch;
	
	virtual void BeginPlay() override;
	
	/** /
	UFUNCTION(BlueprintCallable, Category="Save Data Functions")
	void CheckSaveFiles(UUserWidget* SaveFileWidget);
	/**/
	
	/* Pure C++ Interface Functions for Saving and Loading Data */
	
	virtual void SaveData(const uint8 Slot = 0) override;
	
	virtual void LoadData(const uint8 Slot = 0) override;
	
	virtual void SetActiveCharacter(const int32 NewSlot, AActor* NewCharacter) override;
	
	virtual void LoadLevel(TSoftObjectPtr<UWorld> NewLevel) override;
	
	virtual void UnloadLevel() override;
	
protected:
	void SaveDataAsync(const uint8 Slot = 0);
	
	void OnSaveDataSaved(const FString& SlotName, const int32 UserIndex, bool bSuccess);
	
	void LoadDataAsync(const uint8 Slot = 0);
	
	void OnSaveDataLoaded(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedSaveGame);
	
	UFUNCTION()
	void OnLevelLoaded();
	
	UFUNCTION()
	void OnLevelUnloaded();
};
