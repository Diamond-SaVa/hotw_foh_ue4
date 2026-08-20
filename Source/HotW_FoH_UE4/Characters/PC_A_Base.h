// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HotW_FoH_UE4/Enums/ECharacterEnums.h"
#include "HotW_FoH_UE4/Interfaces/StateSetters.h"
#include "PC_A_Base.generated.h"

class UInputComponent;

// A base class for characters that can move in the world. This class tier is often used for neutral NPCs. 
UCLASS()
class HOTW_FOH_UE4_API APC_A_Base : public ACharacter, public IStateSetters
{
	GENERATED_BODY()
	
protected:	
	// Animation Montage for Character's Jump
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Assets")
	UAnimMontage* AM_Jump;
	
	// Animation Montage for Character's Jump
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Assets")
	UAnimMontage* AM_Landing;
	
	// Movement Property for the Walking Movement Speed 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	float MovementWalkMinSpeed = 200.0f;
	
	// Movement Property for the Running Movement Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	float MovementWalkMaxSpeed = 460.f;
	
	// Movement Property for the Normal Swimming Movement Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	float MovementSwimMinSpeed = 100.0f;
	
	// Movement Property for the Speed-Up Swimming Movement Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	float MovementSwimMaxSpeed = 300.f;
	
	// Movement Property for Movement Control on the Air
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	float MovementAirControl = 1.0f;
	
	// Gravity under normal circumstances
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	float GravityDefault = 1.5f;
	
	// Gravity when colliding with a wall
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	float GravityOnWall = 0.25f;
	
	// Vector struct to keep where the player is pushing or pulling their joysticks or D-Pads
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Input Data")
	float StickDirection;
	
	// Remembers the current state of the character's movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	EMovementState MovementState;
	
	// Remembers if the character is standing, walking or running
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	ESpeedState SpeedState;
	
	// Remembers if the character is doing a specific action
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	EActionState ActionState;

	/* All Class and Game Initializers */
public:
	// Sets default values for this character's properties
	APC_A_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	
	/* Virtual Overridable functions ranging from Event Implementations to Interfaces */
protected:
	// OnJumped Implementation
	virtual void OnJumped_Implementation() override;
	
	// Called when apex of jump was reached
	virtual void NotifyJumpApex() override;
	
	// Event called when character lands on the ground
	virtual void Landed(const FHitResult& Hit) override;
	
	/* Pure C++ Interface Implementation of Setters */
	
	// Class function to manage Movement State changes the Anim Instance's behavior.
	virtual void SetMovementState(const EMovementState NewState) override;
	
	// Class function to manage Action State that locks character from performing other actions or
	// behaviors from other actors, like avoiding damage from the Dodge State.
	virtual void SetActionState(const EActionState NewState) override;
	
	// Class function to manage Speed State changes with anim instance as well as the speed the character is going.
	virtual void SetSpeedState(const ESpeedState NewState) override;
	
	virtual void SetBoolGoingUp(const bool IsGoingUp) override;
	
	/* Public Actions that AI Trees are able to reach */
public:
	// Movement Input of the Character
	void Move(float Value);
	
	// Setup when character stops moving
	void MoveEnd();
	
	// Function for the Character to Jump
	virtual void JumpStart();
	
	// Function for the Character to Stop Jumping
	virtual void JumpEnd();
	
	/* Functions only used by the class itself as well as subclasses inherited by this class */
	
	// Play specific Anim Montage only if a Skeletal Mesh with an AnimInstance still exists
	void PlayAnimMontage_Safe(UAnimMontage* AnimMontage, FName SectionName = FName("Default"), float InPlayRate = 1.0f);
	
protected:
	// Function for Line Tracing Detection
	void LineTraceResponse(const FVector& Direction, const float Distance, FHitResult& Hit, 
		const ECollisionChannel CollisionChannel, const FCollisionQueryParams& QueryParams, bool& bResult) const;
	
	void SetTerminalVelocity(const float NewTerminal);
	
	void StopTerminalVelocity();
	
	void PrintDebug(const FString& Message, float PrintTime = 2.0f);
	
	/* Public Getters and Setters that can be FORCEINLINE */
public:
	// Gets the Stick Direction
	FORCEINLINE float GetStickDirection() const { return StickDirection; }
	
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
	
	// Get the Movement State
	FORCEINLINE EMovementState GetMovementState() const { return MovementState; }
	
	// Get the Speed State
	FORCEINLINE ESpeedState GetSpeedState() const { return SpeedState; }
	
	// Movement Property for the Walking Movement Speed 
	FORCEINLINE float GetMovementWalkMinSpeed() const { return MovementWalkMinSpeed; }
	
	// Movement Property for the Running Movement Speed
	FORCEINLINE float GetMovementWalkMaxSpeed() const { return MovementWalkMaxSpeed; }
	
	// Movement Property for the Normal Swimming Movement Speed
	FORCEINLINE float GetMovementSwimMinSpeed() const { return MovementSwimMinSpeed; }
	
	// Movement Property for the Speed-Up Swimming Movement Speed
	FORCEINLINE float GetMovementSwimMaxSpeed() const { return MovementSwimMaxSpeed; }
};
