// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "HotW_FoH_UE4/Interfaces/PCInterface.h"
#include "PC_Control.generated.h"

class APC_C_Prince;
class UInputMappingContext;
class UInputAction;

UCLASS()
class HOTW_FOH_UE4_API APC_Control : public APlayerController, public IPCInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Character Load Data")
	TArray<TWeakObjectPtr<APC_C_Prince>> PCharacterArray;
	
	// Input Action for Movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Input Assets")
	UInputAction* IA_Move_Left;
	
	// Input Action for Movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Input Assets")
	UInputAction* IA_Move_Right;
	
	// Input Action for Jumping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Input Assets")
	UInputAction* IA_Jump;
	
	// Input Action for Attacking
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Input Assets")
	UInputAction* IA_Attack;
	
	// Input Action used for Interactions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Input Assets")
	UInputAction* IA_Interact;
	
	// Input Action used for Interactions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Input Assets")
	UInputAction* IA_LookUp;
	
	// Input Action used for Crouch
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Input Assets")
	UInputAction* IA_LookDown;
	
	// Input Action used for Dodging and Dashing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Input Assets")
	UInputAction* IA_Dodge;
	
	// Input Action used for Switching Characters
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Input Assets")
	UInputAction* IA_Switch;
	
	// Input Mapping Context for regular inputs 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Input Assets")
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="User Widgets")
	TSubclassOf<UUserWidget> MainWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="User Widgets")
	class UUW_CharacterSwitch* MainWidgetInstance;
	
	FTimerHandle ReenableSwitchTimerHandle;
	
	// ID to set the Current Character Slot variable when character lands
	UPROPERTY(VisibleAnywhere, Category="Character Input Data")
	int32 NextCharacterSlot = 0; 
	
	// The Slot of the Character to control in the Array
	UPROPERTY(VisibleAnywhere, Category="Character Input Data")
	uint8 CurrentCharacterSlot = 0;
	
	bool CanSwitchCharacter = true;
	
	UPROPERTY(VisibleAnywhere, Category="Character Input Data")
	bool MovingLeft = false;
	
	UPROPERTY(VisibleAnywhere, Category="Character Input Data")
	bool MovingRight = false;

	// Sets default values for this actor's properties
	APC_Control();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	virtual void SetupInputComponent() override;
	
	/* Input Action Bound Functions */
	
	/// Set to Input Action's Attack on Start
	UFUNCTION()
	void ActionAttack();
	
	/// Movement Input of the Character from input Trigger
	UFUNCTION()
	void ActionMovement(const FInputActionValue& Value);
	
	/// Movement Input of the Character from input Trigger
	UFUNCTION()
	void ActionMovementLeft(const FInputActionValue& Value);
	
	/// Movement Input of the Character from input Trigger
	UFUNCTION()
	void ActionMovementRight(const FInputActionValue& Value);
	
	/// Setup when movement stops from input Complete
	UFUNCTION()
	void ActionMovementStop();
	
	/// Setup when movement stops from input Complete
	UFUNCTION()
	void ActionMovementLeftStop();
	
	/// Setup when movement stops from input Complete
	UFUNCTION()
	void ActionMovementRightStop();
	
	/// Function for the Character to Jump
	UFUNCTION()
	void ActionJumpStart();
	
	/// Function for the Character to Jump
	UFUNCTION()
	void ActionJumpHold();
	
	/// Function for the Character to Stop Jumping
	UFUNCTION()
	void ActionJumpEnd();
	
	UFUNCTION()
	void ActionStopWallSliding();
	
	UFUNCTION()
	void ActionInteractStart();
	
	UFUNCTION()
	void ActionPressUpStart();
	
	UFUNCTION()
	void ActionPressUpEnd();
	
	UFUNCTION()
	void ActionPressDownStart();
	
	UFUNCTION()
	void ActionPressDownEnd();
	
	UFUNCTION()
	void ActionDodgeStart();
	
	UFUNCTION()
	void ActionBufferSwitch();
	
	UFUNCTION()
	void ActionSwitchCharacter();
	
	void SwitchCharacterAfterBuffer();
	
	void SwitchCharacterFromLanding();
	
	void SwitchCharacterProcess();
	
	void ActivateSwitchBlockTimer();
	
	void CharacterSetUpFromBeginPlay(APC_C_Prince* PlayableCharacter, int32 CharacterID);
	
	void ReenableSwitch();
	
	void SendCharacterToManager();
	
protected:
	/* Pure Virtual Interface Functions */
	virtual void RegisterCharacterToControllerArray(APC_C_Prince* PlayableCharacter, const int32 CharacterID) override;
	
	virtual void CheckIfCouldSwitchOnLanded() override;
};
