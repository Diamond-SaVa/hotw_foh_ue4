// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "PC_Control.generated.h"

class APC_C_Prince;
class UInputMappingContext;
class UInputAction;

UCLASS()
class HOTW_FOH_UE4_API APC_Control : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Load Data")
	TArray<APC_C_Prince*> PCArray;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Load Data")
	TWeakObjectPtr<APC_C_Prince> ThePC;
	
	// Input Action for Movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Input Assets")
	UInputAction* IA_Move;
	
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
	UInputAction* IA_Crouch;
	
	// Input Action used for Dodging and Dashing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Input Assets")
	UInputAction* IA_Dodge;
	
	// Input Action used for Switching Characters
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Input Assets")
	UInputAction* IA_Switch;
	
	// Input Mapping Context for regular inputs 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Input Assets")
	UInputMappingContext* InputMappingContext;
	
	FTimerHandle ReenableSwitchTimerHandle;
	
	int32 CurrentCharacterSlot;
	
	// 0 is true, 1 is false
	int32 CanSwitchCharacter;

	// Sets default values for this actor's properties
	APC_Control();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	virtual void SetupInputComponent() override;
	
	void CharacterSetUpFromBeginPlay(APC_C_Prince* PlayableCharacter, int32 CharacterID);
	
	void ReenableSwitch();
	
	/* Input Action Bound Functions */
	
	/// Set to Input Action's Attack on Start
	UFUNCTION()
	void ActionAttack();
	
	/// Movement Input of the Character from input Trigger
	UFUNCTION()
	void ActionMovement(const FInputActionValue& Value);
	
	/// Setup when movement stops from input Complete
	UFUNCTION()
	void ActionMovementStop();
	
	/// Function for the Character to Jump
	UFUNCTION()
	void ActionJumpStart();
	
	/// Function for the Character to Stop Jumping
	UFUNCTION()
	void ActionJumpEnd();
	
	UFUNCTION()
	void ActionStopWallSliding();
	
	UFUNCTION()
	void ActionWallAndLedgeDetection();
	
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
	void ActionSwitchCharacter();
	
	void SendCharacterToManager();
	
	bool CanAssignControls() const;
};
