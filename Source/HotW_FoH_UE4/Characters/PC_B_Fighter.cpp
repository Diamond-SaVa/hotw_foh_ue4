// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "PC_B_Fighter.h"


#include "PropertyPathHelpers.h"
#include "HotW_FoH_UE4/Components/AC_Stats.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APC_B_Fighter::APC_B_Fighter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	StatsComponent = CreateDefaultSubobject<UAC_Stats>(FName("StatsComponent"));
	
	HitState = EHitState::EHit_Damage;
}

// Called when the game starts or when spawned
void APC_B_Fighter::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void APC_B_Fighter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APC_B_Fighter::Landed(const FHitResult& Hit)
{
	// If the character is in the middle of an Action, like attacking or dodging,
	if (ActionState != EActionState::EAct_NilState)
	{
		// Call directly its ACharacter::Landed event to proceed with physics and movement mode behaviors
		ACharacter::Landed(Hit);
		// and interrupt further code
		return;
	}
	
	// Otherwise, if the character is not doing any special action, then proceed with the hierarchy of Landed calls.
	Super::Landed(Hit);
}

void APC_B_Fighter::TakeDamage(const int32 Damage)
{
	bool StatsValid = IsValid(StatsComponent);
	
	// Checks if the character is Damageable or if it's not already on a Hurt State or K.O.ed
	if (HitState == EHitState::EHit_NoDamage || ActionState == EActionState::EAct_HurtState ||
		ActionState == EActionState::EAct_KOed_State || StatsValid == false)
	{		
		return;
	}
	
	// Deals damage to the Stats Component
	StatsComponent->DamageHP(Damage);

	const bool IsAlive = StatsComponent->GetHP() > 0;
	
	HitState = IsAlive ? EHitState::EHit_Damage : EHitState::EHit_NoDamage;
	
	UAnimMontage* AnimToPlay = IsAlive ? AM_Damage : AM_KO;
	
	// Play damage animation
	PlayAnimMontage_Safe(AnimToPlay);
}

void APC_B_Fighter::KnockbackFromSource(const FVector& DamageSourceLocation, const float KnockbackPower)
{	
	// Sets a starting name for the Anim Montage Section to play
	FName SectionName = FName("Default");
	
	// Checks if the Knockback power surpasses a certain threshold to change the Animation to play
	if (KnockbackPower > 1000.0f)
	{
		const FString SectionText = "Launched";
		SectionName = FName(*SectionText);
	}
	
	// Plays the Anim Montage using the set Section.
	PlayAnimMontage_Safe(AM_Jump, SectionName);
	
	KnockbackFromSource(DamageSourceLocation, KnockbackPower);
}

void APC_B_Fighter::SphereTraceDamage(const FVector& StartLocation, const FVector& EndLocation, 
	TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const float Radius)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp == nullptr)
	{
		return;
	}
	
	// Prepare the array with FHitResults
	TArray<FHitResult> HitResults;
	
	// And the params removing this character from the detection of the sweep
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(), 
			StartLocation, 
			EndLocation,
			Radius, 
			ObjectTypes,					// Object Type
			false,							// bTraceComplex
			{this}, 
			EDrawDebugTrace::ForDuration,	// <--- Enforces the Debug Drawing
			HitResults, 
			true,							// bIgnoreSelf
			FLinearColor::Red,				// Trace Color
			FLinearColor::Green,			// Hit Color
			1.0f							// Draw Duration (Seconds)
		);
		
		//World->SweepMultiByChannel(HitResults, StartLocation, EndLocation, FQuat::Identity, TraceChannel, 
		//Shape, QueryParams);
	
	if (bHit == false || HitResults.Num() == 0)
	{
		return;
	}
	
	const FName HitTag = FName("HIT");
	
	FString DebugText = "";
	
	for (int i = 0; i < HitResults.Num(); ++i)
	{
		if (const UPrimitiveComponent* Comp = HitResults[i].GetComponent())
		{
			DebugText = TEXT("Found Component " + Comp->GetName());
			
			PrintDebug(DebugText);
			
			if (Comp->ComponentHasTag(HitTag))
			{
				DebugText = (Comp->GetName() + " has tag");
			
				PrintDebug(DebugText);
				
				if (IDamageable* DamageableActor = Cast<IDamageable>(Comp->GetOwner()))
				{
					DebugText = ("Actor does have IDamageable");
			
					PrintDebug(DebugText);
					
					DamageableActor->TakeDamage(5);
					
					return;
				}
			}
		}
	}
}

void APC_B_Fighter::Attack()
{
	// Checks if the Character's Movement State is not currently attacking, hurting or even K.O.ed
	if (ActionState != EActionState::EAct_NilState)
	{
		return;
	}
	
	SetActionState(EActionState::EAct_AttackState);

	// Sets the AtkCount to the Section Name to call out
	const FString Count = FString::FromInt(AtkCounter);
	const FName MontageSectionName = FName(*Count);
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, Count);
	
	// Plays the AnimMontage from the set Section Name
	PlayAnimMontage_Safe(AM_Attack, MontageSectionName);
	
	AtkCounter++;
	
	if (AtkCounter >= AtkCounterMax)
	{
		AtkCounter = 0;
	}
}

