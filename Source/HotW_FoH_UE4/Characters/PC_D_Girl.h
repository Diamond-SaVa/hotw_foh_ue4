// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "PC_C_Prince.h"
#include "PC_D_Girl.generated.h"

UCLASS()
class HOTW_FOH_UE4_API APC_D_Girl : public APC_C_Prince
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Character Components")
	class UAC_ShootingComponent* ShootingComponent;

public:	
	// Sets default values for this character's properties
	APC_D_Girl();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnJumped_Implementation() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/* Functions to Override for Girl */
	virtual void JumpStart() override;
	
	virtual void Attack() override;
	
	virtual void LedgeGrabStart() override;
	
	virtual void WallSlideStart() override;
	
	/* Actions to Bind */
	
	/* Functions Unique to Girl Character */
	void WallJumpAnim();
	
	void WallJumpLaunch(const FVector& LaunchDirection);
};
