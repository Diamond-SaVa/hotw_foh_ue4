// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "AC_Stats.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "HotW_FoH_UE4/GameData/GI_GameData.h"


// Sets default values for this component's properties
UAC_Stats::UAC_Stats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAC_Stats::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UAC_Stats::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		// Stop the timer immediately so it doesn't fire during garbage collection
		World->GetTimerManager().ClearTimer(RecoveryTimerHandle);
	}
	
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		if (World != nullptr)
		{
			// Get world's GameMode to call its Interface that adds EXP to the player
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

/** /
// Called every frame
void UAC_Stats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
/**/

void UAC_Stats::UpdateMaxValues()
{
	if (const UGI_GameData* GameData = Cast<UGI_GameData>(GetWorld()->GetGameInstance()))
	{
		MaxHP = GameData->GetBaseHP() * GameData->GetHealthLevel();
		
		MaxEP = GameData->GetBaseEP() * GameData->GetEnergyLevel();
	}
}

void UAC_Stats::RecoverPoints()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
		return;
	
	if (EP < MaxEP)
	{
		EP = FMath::Clamp(EP + 1, 0, MaxEP);
		
		if (EP == MaxEP)
		{
			World->GetTimerManager().ClearTimer(RecoveryTimerHandle);
		}
	}
}

void UAC_Stats::SpendEP(int32 Value)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
		return;
	
	EP = FMath::Clamp(EP - Value, 0, MaxEP);
	
	if (World->GetTimerManager().IsTimerActive(RecoveryTimerHandle) == false && EP < MaxEP)
	{
		World->GetTimerManager().SetTimer(
			RecoveryTimerHandle,
			this,
			&UAC_Stats::RecoverPoints,
			RecoveryInterval,
			true
			);
	}
}

