// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "HotW_FoH_UE4/Characters/PC_B_Fighter.h"
#include "NPC_Enemy_Static.generated.h"

UCLASS()
class HOTW_FOH_UE4_API ANPC_Enemy_Static : public APC_B_Fighter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_Enemy_Static();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
