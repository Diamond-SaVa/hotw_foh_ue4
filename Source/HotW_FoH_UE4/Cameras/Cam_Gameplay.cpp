// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "Cam_Gameplay.h"

#include "HotW_FoH_UE4/Controllers/PC_Control.h"
#include "HotW_FoH_UE4/Managers/GM_Manager.h"


// Sets default values
ACam_Gameplay::ACam_Gameplay()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetTickGroup(TG_PostUpdateWork);
}

// Called when the game starts or when spawned
void ACam_Gameplay::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		return;
	}
	
	if (AGM_Manager* GM = Cast<AGM_Manager>(World->GetAuthGameMode()))
	{
		GM->OnCharacterSwitch.AddDynamic(this, &ACam_Gameplay::SetTargetActor);
	}
	;
	if (APC_Control* PC = Cast<APC_Control>(World->GetFirstPlayerController()))
	{
		PC->SetViewTargetWithBlend(this, 1.0f, VTBlend_EaseOut, 2.0f);
	}
}

// Called every frame
void ACam_Gameplay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (TargetActor.IsValid() == false)
	{
		return;	
	}
	
	/* Set the new location of the Camera at runtime */
	
	// Prepare the smooth velocity at which the camera will follow each axis, as well as constant expression values.
	const float FollowLerpX = DeltaTime * CameraSpeedX;
	const float FollowLerpZ = DeltaTime * CameraSpeedZ;
	
	// Obtains the current location of the Camera
	const FVector CurrentLocation = GetActorLocation();
	
	// And then obtains the Location of the Target Actor and modify each individual value to accomodate for the next
	// location of the Camera.
	FVector TargetLocation = TargetActor.Get()->GetActorLocation();
	TargetLocation.X += TargetActor->GetActorForwardVector().X * OffsetFromTargetX;
	TargetLocation.Y += OffsetFromTargetY;
	TargetLocation.Z += OffsetFromTargetZ;
	
	// And finally prepare and set the final Location by Interpolating each appropriate value into their next
	// value from the TargetLocation, and set it to this Camera's Location
	FVector NewLocation;
	NewLocation.X = FMath::LerpStable(CurrentLocation.X, TargetLocation.X, FollowLerpX);
	NewLocation.Y = TargetLocation.Y;
	NewLocation.Z = FMath::LerpStable(CurrentLocation.Z, TargetLocation.Z, FollowLerpZ);
	SetActorLocation(NewLocation);
}

void ACam_Gameplay::SetTargetActor(AActor* NewTargetActor)
{
	TargetActor = NewTargetActor;
}

