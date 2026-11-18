// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "PC_C_Prince.h"
#include "PC_D_Girl.generated.h"

UCLASS()
class HOTW_FOH_UE4_API APC_D_Girl : public APC_C_Prince
{
	GENERATED_BODY()

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
	
	// Called when apex of jump was reached
	virtual void NotifyJumpApex() override;
	
	virtual void JumpStart() override;
	
	virtual void JumpHold() override;
	
	virtual void JumpEnd() override;
	
	virtual void Attack() override;
	
	virtual void SwitchMaskTo(EMaskState MaskValue) override;
	
	virtual void SetProjectileOfMask(const TSoftClassPtr<class AA_Projectile>& NewProjectile) override;
	
	/* Actions to Bind */
	
	/* Functions Unique to Girl Character */
	void WallJumpAnim();
	
	void WallJumpLaunch(const FVector& LaunchDirection);
};
