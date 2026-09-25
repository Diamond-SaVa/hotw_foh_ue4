// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_Stats.generated.h"

/*
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HOTW_FOH_UE4_API UAC_Stats : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	// Rate at which this Component's Tick runs on the thread.
	// Used for HP and EP recovery purposes.
	UPROPERTY(Category = "Recovery Data", EditAnywhere, BlueprintReadWrite)
	float RecoveryInterval = 2.0f;
	
	// Amount of EP recovered each time until EP is full again.
	UPROPERTY(Category = "Recovery Data", EditAnywhere, BlueprintReadWrite)
	int32 RecoveryEP = 3;
	
	// Max HP calculated with the GameInstance's HealthLevel variable
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadOnly)
	int32 MaxHP = 30;
	
	// HP that is always changing when being hit or healing, clamped to MaxHP
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadOnly)
	int32 HP = 30;
	
	// Max EP calculated with the GameInstance's EnergyLevel variable
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadOnly)
	int32 MaxEP = 30;
	
	// EP that is always changing when being hit or healing, clamped to MaxEP
	UPROPERTY(Category = "Player Data", VisibleAnywhere, BlueprintReadOnly)
	int32 EP = 30;
	
	// Handler for managing Recovery Timer
	UPROPERTY()
	FTimerHandle RecoveryTimerHandle;

public:
	// Sets default values for this component's properties
	UAC_Stats();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called when Actor or Actor Component is about to be destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/** /
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	/**/
	
	// Updates MaxHP and MaxEP based on the stored Base values and Levels obtained by the player.
	void UpdateMaxValues();
	
	// Recovers EP as dictated by the parameters.
	void RecoverPoints();
	
	// Make the character heal EP
	void SpendEP(int32 Value);
	
	FORCEINLINE int32 GetHP() const { return HP; }
	
	FORCEINLINE int32 GetEP() const { return EP; }
	
	// Make the character Heal HP
	FORCEINLINE void HealHP(int32 Value) { HP = FMath::Clamp(HP + Value, 0, MaxHP); }
	
	// Make the character take damage
	FORCEINLINE void DamageHP(int32 Value) { HP = FMath::Clamp(HP - Value, 0, MaxHP); }
	
	// Make the character heal EP
	FORCEINLINE void EarnEP(int32 Value) { EP = FMath::Clamp(EP + Value, 0, MaxEP); }
	
	// Change the rate at which the character recovers EP
	FORCEINLINE void SetRecoveryRate(float NewRate) { RecoveryInterval = NewRate; }
};
