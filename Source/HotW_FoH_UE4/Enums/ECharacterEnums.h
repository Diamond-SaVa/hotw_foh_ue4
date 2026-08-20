// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "ECharacterEnums.generated.h"

/**
 * List of States for the Animation Blueprint to filter animations.
 */
UENUM(BlueprintType)
enum class EMovementState : uint8
{
	ECS_GroundState	= 0	UMETA(DisplayName = "On the ground"),
	ECS_CrouchState	= 1	UMETA(DisplayName = "Crouching"),
	ECS_AirState	= 2	UMETA(DisplayName = "In the air"),
	ECS_WallState	= 3 UMETA(DisplayName = "Sliding on Wall"),
	ECS_LedgeState	= 4	UMETA(DisplayName = "Hands on the ledge"),
	ECS_SwimState	= 3	UMETA(DisplayName = "Is Swimming")
};

/**
 * List of States for the Animation Blueprint to filter animations.
 */
UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAct_NilState			= 0	UMETA(DisplayName = "No Action"),
	EAct_DodgeState			= 1	UMETA(DisplayName = "Dodging or Dashing"),
	EAct_PerfectDodgeState	= 2	UMETA(DisplayName = "Perfect Dodging"),
	EAct_AttackState		= 3 UMETA(DisplayName = "Attacking"),
	EAct_SideAttackState	= 4 UMETA(DisplayName = "Side Attacking"),
	EAct_WallJumpState		= 5 UMETA(DisplayName = "Wall Jumping"),
	EAct_HurtState			= 6 UMETA(DisplayName = "Hurting"),
	EAct_KOed_State			= 7	UMETA(DisplayName = "Knocked Down")
};

/**
 * List of States that defines incoming damage behavior.
 */
UENUM(BlueprintType)
enum class EHitState : uint8
{
	EHit_NoDamage	= 0	UMETA(DisplayName = "Can not be damaged"),
	EHit_Damage		= 1	UMETA(DisplayName = "Can be damaged")
};

/**
 * List of States that defines incoming damage behavior.
 */
UENUM(BlueprintType)
enum class ESpeedState : uint8
{
	ESP_Stop	= 0	UMETA(DisplayName = "Is standing still"),
	ESP_Walk	= 1	UMETA(DisplayName = "Is walking"),
	ESP_Run		= 2	UMETA(DisplayName = "Is running")
};