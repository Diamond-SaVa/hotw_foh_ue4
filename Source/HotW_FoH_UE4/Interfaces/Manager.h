// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Manager.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HOTW_FOH_UE4_API IManager
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, Category = "Data Manager Function")
	virtual void SaveData(const uint8 Slot = 0) = 0;
	
	UFUNCTION(BlueprintCallable, Category = "Data Manager Function")
	virtual void LoadData(const uint8 Slot) = 0;
	
	virtual void SetActiveCharacter(const int32 NewSlot, AActor* NewCharacter) = 0;
	
	virtual void LoadLevel(TSoftObjectPtr<UWorld> NewLevel) = 0;
	
	virtual void UnloadLevel() = 0;
};
