// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "NPC_Enemy_Static.h"


// Sets default values
ANPC_Enemy_Static::ANPC_Enemy_Static()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANPC_Enemy_Static::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC_Enemy_Static::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANPC_Enemy_Static::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

