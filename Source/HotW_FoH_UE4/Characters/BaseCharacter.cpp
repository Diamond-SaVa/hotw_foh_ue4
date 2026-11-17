// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "BaseCharacter.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();
}

void ABaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}

void ABaseCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
}

void ABaseCharacter::InteractStart()
{
}

void ABaseCharacter::MoveStart(float Direction)
{
}

void ABaseCharacter::MoveHold(float Direction)
{
}

void ABaseCharacter::MoveStop()
{
}

void ABaseCharacter::JumpStart()
{
}

void ABaseCharacter::JumpHold()
{
}

void ABaseCharacter::JumpEnd()
{
}

void ABaseCharacter::AttackStart()
{
}

void ABaseCharacter::AttackHold()
{
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::DodgeStart()
{
}

void ABaseCharacter::DodgeHold()
{
}

void ABaseCharacter::DodgeEnd()
{
}

void ABaseCharacter::ActivateCharacter(AController* NewController)
{
}

void ABaseCharacter::DeactivateCharacter()
{
}

void ABaseCharacter::OnLedgeGrabStart()
{
}

void ABaseCharacter::OnLedgeGrabStop()
{
}

void ABaseCharacter::OnWallSlideStart()
{
}

void ABaseCharacter::OnWallSlideStop()
{
}

void ABaseCharacter::SetMovementState(const EMovementState NewState)
{
}

void ABaseCharacter::SetActionState(const EActionState NewState)
{
}

void ABaseCharacter::SetSpeedState(const ESpeedState NewState)
{
}

void ABaseCharacter::SetBoolGoingUp(const bool IsGoingUp)
{
}

void ABaseCharacter::CharacterVelocityInterface(const FVector& NewVelocity)
{
}

void ABaseCharacter::CharacterVelocityInterface(const EDirection AxisDirection, const float NewVelocity)
{
}

void ABaseCharacter::DodgeInterface(const float DodgePower, const float DeltaSeconds, const EDirection DirectionEnum)
{
}

void ABaseCharacter::LedgeGrabStart()
{
}

void ABaseCharacter::LedgeJumpInterface()
{
}

void ABaseCharacter::MoveInputInterface(const float Direction)
{
}

void ABaseCharacter::WallSlideStart()
{
}

void ABaseCharacter::SphereTraceDamage(const FVector& StartLocation, const FVector& EndLocation,
	TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, float Radius)
{
}

void ABaseCharacter::TakeDamage(const int32 Damage)
{
}

void ABaseCharacter::KnockbackFromSource(const FVector& DamageSourceLocation, const float KnockbackPower)
{
}

