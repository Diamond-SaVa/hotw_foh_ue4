// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "PC_D_Girl.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HotW_FoH_UE4/Components/AC_ShootingComponent.h"

// Sets default values
APC_D_Girl::APC_D_Girl()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ShootingComponent = CreateDefaultSubobject<UAC_ShootingComponent>(FName("TheShootingComponent"));
}

// Called when the game starts or when spawned
void APC_D_Girl::BeginPlay()
{
	CharacterID = 1;
	
	Super::BeginPlay();
}

void APC_D_Girl::OnJumped_Implementation()
{
	CoyoteJumpDisable();
	
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		// Creates the Section Name as a default variable to call the Anim Montage animation from the ground
		FName SectionName = FName("Default");
	
		// If the Trace doesn't hit, then the Section Name is changed 
		if (JumpCurrentCount >= JumpMaxCount)
		{
			const FString SectionText = "Air";
			SectionName = FName(*SectionText);
		}
		
		PrintDebug(SectionName.ToString());
	
		PlayAnimMontage_Safe(AM_Jump, SectionName);
	
		// Resets the bNotifyApex to true for each jump
		UCharacterMovementComponent* MoveComp = GetCharacterMovement();
		MoveComp->bNotifyApex = true;
	
		SetActionState(EActionState::EAct_NilState);
	
		PrintDebug(FString::Printf(TEXT("Jump : %d - Max Count : %d"), JumpCurrentCount, JumpMaxCount));
	
		ACharacter::OnJumped_Implementation();
		
		return;
	}
	
	Super::OnJumped_Implementation();
}

// Called to bind functionality to input
void APC_D_Girl::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APC_D_Girl::JumpStart()
{
	switch (MovementState)
	{
	case EMovementState::ECS_WallState:
		WallJumpAnim();
		return;
		
	case EMovementState::ECS_LedgeState:
		LedgeUpAnim();
		return;
		
	default:
		break;
	}
	
	Super::JumpStart();
}

void APC_D_Girl::Attack()
{
	if (ShootingComponent == nullptr)
	{
		return;
	}
	
	ShootingComponent->ShootNextAvailableProjectile();
}

void APC_D_Girl::WallJumpAnim()
{
	if (ActionState != EActionState::EAct_NilState && MovementState != EMovementState::ECS_WallState)
	{
		return;
	}
	
	SetActionState(EActionState::EAct_WallJumpState);
	SetMovementState(EMovementState::ECS_AirState);
	
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += 180.0f;
	SetActorRotation(Rotation);
	
	PlayAnimMontage_Safe(AM_Jump, FName("Wall"));
	
	FVector LaunchVelocity = GetActorForwardVector() * 500.0f;
	LaunchVelocity.Z = 750.0f;
	
	WallJumpLaunch(LaunchVelocity);
}

void APC_D_Girl::WallJumpLaunch(const FVector& LaunchDirection)
{
	LaunchCharacter(LaunchDirection, true, true);
}

