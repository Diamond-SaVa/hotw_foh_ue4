// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HotW_FoH_UE4/Enums/ECharacterEnums.h"
#include "HotW_FoH_UE4/Interfaces/StateSetters.h"
#include "AC_WallAndLedgeComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HOTW_FOH_UE4_API UAC_WallAndLedgeComponent : public UActorComponent, public IStateSetters
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wall Trace Modifiers")
	float WallTraceHeightModifier = 0.75f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wall Trace Modifiers")
	float WallHalfHeightMod;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Wall Trace Modifiers")
	float WallRadiusMod;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wall Trace Modifiers")
	EActionState CompActionState = EActionState::EAct_NilState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wall Trace Modifiers")
	EMovementState CompMovementState = EMovementState::ECS_GroundState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wall Trace Modifiers")
	bool bWallDetectTop;

public:
	// Sets default values for this component's properties
	UAC_WallAndLedgeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	/* Pure Virtual Interface Functions */
	
	virtual void SetMovementState(const EMovementState NewState) override;
	
	virtual void SetActionState(const EActionState NewState) override;
	
	/* Empty Pure Virtual Interface Functions  */
	
	virtual void SetSpeedState(const ESpeedState NewState) override;
	
	virtual void SetBoolGoingUp(const bool IsGoingUp) override;
	
	/* Class Functions */
	
	void WallDetection();
	
	void LedgeDetection();
	
	void TraceFunction(AActor* PCOwner, UWorld* World, bool bTopTrace, bool& bTraceResult);
	
	void ResetOnLanded();
};
