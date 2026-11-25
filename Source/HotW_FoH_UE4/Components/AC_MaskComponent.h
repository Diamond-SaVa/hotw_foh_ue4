// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HotW_FoH_UE4/Enums/ECharacterEnums.h"
#include "AC_MaskComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HOTW_FOH_UE4_API UAC_MaskComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category=AC_MaskComponent)
	TArray<TSoftObjectPtr<UStaticMesh>> MaskMeshArray;
	
	UPROPERTY(EditAnywhere, Category=AC_MaskComponent)
	TArray<TSoftObjectPtr<class AA_Projectile>> ProjectilePerMaskArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects References")
	class UNiagaraSystem* NS_MaskEffect;
	
	UPROPERTY(VisibleAnywhere, Category=AC_MaskComponent)
	EMaskState MaskState;
	
public:
	FORCEINLINE EMaskState GetMaskState() const { return MaskState; }
	
public:
	// Sets default values for this component's properties
	UAC_MaskComponent();
	
	void SwitchMask(EMaskState NewMaskState);
	
	void UpdateMaskOnCharacter();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
