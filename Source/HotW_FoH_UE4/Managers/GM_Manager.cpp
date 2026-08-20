// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "GM_Manager.h"

#include "Engine/LevelStreamingDynamic.h"
#include "HotW_FoH_UE4/GameData/SG_SaveData.h"
#include "Kismet/GameplayStatics.h"


void AGM_Manager::BeginPlay()
{
	Super::BeginPlay();
}

void AGM_Manager::SaveData(const uint8 Slot)
{
	SaveDataAsync(Slot);
}

void AGM_Manager::LoadData(const uint8 Slot)
{
	LoadDataAsync(Slot);
}

void AGM_Manager::SetActiveCharacter(const int32 NewSlot, AActor* NewCharacter)
{
	CurrentCharacterSlot = NewSlot;
	
	OnCharacterSwitch.Broadcast(NewCharacter);
}

void AGM_Manager::LoadLevel(TSoftObjectPtr<UWorld> NewLevel)
{
	UWorld* TheWorld = GetWorld();
	if (CurrentLevel.IsNull() == true || IsValid(TheWorld) == false)
	{
		return;
	}
	
	bool bSuccess = false;
    
	// Spawn and load the level with the Characters' instances
	LoadedLevelInstance = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
		TheWorld, 
		CurrentLevel, 
		FVector::ZeroVector, 
		FRotator::ZeroRotator, 
		bSuccess
	);

	if (bSuccess && LoadedLevelInstance)
	{
		// Bind the loading completion callback
		LoadedLevelInstance->OnLevelLoaded.AddDynamic(this, &AGM_Manager::OnLevelLoaded);
	}
}

void AGM_Manager::UnloadLevel()
{
	if (IsValid(LoadedLevelInstance) == false || LoadedLevelInstance->IsLevelLoaded() == false)
	{
		return;
	}
	
	// Alter memory state
	LoadedLevelInstance->SetShouldBeLoaded(false);
        
	// Alter rendering visibility state
	LoadedLevelInstance->SetShouldBeVisible(false);
}

void AGM_Manager::SaveDataAsync(const uint8 Slot)
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld == nullptr)
	{
		return;
	}

	const FString SlotName = FString::Printf(TEXT("FOH_Slot_%d"), Slot);
	
	FAsyncSaveGameToSlotDelegate SaveDelegate;
	
	SaveDelegate.BindUObject(this, &AGM_Manager::OnSaveDataSaved);
	
	int32 TargetUserIndex = 0;
	
	if (APlayerController* PC = Cast<APlayerController>(TheWorld->GetFirstPlayerController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			TargetUserIndex = LP->GetControllerId();
		}
	}
	
	UGameplayStatics::AsyncSaveGameToSlot(nullptr, SlotName, TargetUserIndex, SaveDelegate);
}

void AGM_Manager::OnSaveDataSaved(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if (bSuccess == true)
	{
		UE_LOG(LogTemp, Log, TEXT("LOG: Save Data with name %s has been saved successfully."), *SlotName);
		
		// Show UI with save success message.
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR: Save Data with name %s can't be saved."), *SlotName);
		
		// Show UI with save failed message.
	}
}

void AGM_Manager::LoadDataAsync(const uint8 Slot)
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld == nullptr)
	{
		return;
	}
	
	const FString SlotName = FString::Printf(TEXT("FOH_Slot_%d"), Slot);
	
	FAsyncLoadGameFromSlotDelegate LoadSaveDelegate;
	
	LoadSaveDelegate.BindUObject(this, &AGM_Manager::OnSaveDataLoaded);
	
	int32 TargetUserIndex = 0;
	
	if (APlayerController* PC = Cast<APlayerController>(TheWorld->GetFirstPlayerController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			TargetUserIndex = LP->GetControllerId();
		}
	}
	
	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, TargetUserIndex, LoadSaveDelegate);
}

void AGM_Manager::OnSaveDataLoaded(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedSaveGame)
{
	if (LoadedSaveGame == nullptr)
	{
		// Show load data error
		UE_LOG(LogTemp, Error, TEXT("ERROR: %s Can't be found"), *SlotName);
		return;
	}
	
	USG_SaveData* SGInstance = Cast<USG_SaveData>(LoadedSaveGame);
	
	if (SGInstance == nullptr)
	{
		// Show corrupted data error
		UE_LOG(LogTemp, Error, TEXT("%s: SaveFile is not the expected type of USaveGame class %s"), *SlotName, 
			*USG_SaveData::StaticClass()->GetName());
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("LOG: Save File with name %s has been successfully loaded."), *SlotName);
}

void AGM_Manager::OnLevelLoaded()
{
	UE_LOG(LogTemp, Log, TEXT("LOG: Level Loaded."));
}

void AGM_Manager::OnLevelUnloaded()
{
	LoadedLevelInstance = nullptr;
}
