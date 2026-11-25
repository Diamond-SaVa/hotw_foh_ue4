// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputLibrary.h"
#include "GameFramework/PlayerController.h"
#include "HotW_FoH_UE4/Interfaces/PCInterface.h"
#include "PC_Player.generated.h"

struct FInputActionValue;
class ABaseCharacter;
class UInputAction;
/**
 * 
 */
UCLASS()
class HOTW_FOH_UE4_API APC_Player : public APlayerController, public IPCInterface
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Character Load Data")
	TArray<TSoftClassPtr<ABaseCharacter>> CharacterClassArray;
	
	UPROPERTY(EditAnywhere, Category = "Character Load Data")
	TArray<TWeakObjectPtr<AActor>> PCharacterArray;
	
	UPROPERTY(EditAnywhere, Category = "Character Input Assets")
	UInputMappingContext* InputMappingContext;
	
	// Input Action for Movement
	UPROPERTY(EditAnywhere, Category = "Character Input Assets")
	UInputAction* IA_Move_Left;
	
	// Input Action for Movement
	UPROPERTY(EditAnywhere, Category = "Character Input Assets")
	UInputAction* IA_Move_Right;
	
	UPROPERTY(VisibleAnywhere, Category="Character Input Data")
	uint8 CurrentCharacterIndex = 0;
	
	UPROPERTY(VisibleAnywhere, Category="Character Input Data")
	bool MovingLeft = false;
	
	UPROPERTY(VisibleAnywhere, Category="Character Input Data")
	bool MovingRight = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	virtual void SetupInputComponent() override;
	
protected:
	
	/* Controller Functions */
	
	UFUNCTION()
	void ActionMovementLeftStart();
	
	/// Setup when movement stops from input Complete
	UFUNCTION()
	void ActionMovementLeftStop();
	
	/// Movement Input of the Character from input Trigger
	UFUNCTION()
	void ActionMovementHold(const FInputActionValue& Value);
	
	UFUNCTION()
	void ActionMovementRightStart();
	
	/// Setup when movement stops from input Complete
	UFUNCTION()
	void ActionMovementRightStop();
	
	/* IPCInterface Functions */
	
	virtual void CheckIfCouldSwitchOnLanded() override;
	
	virtual void RegisterCharacterToControllerArray(AActor* PlayableCharacter, const int32 CharacterID) override;

};
