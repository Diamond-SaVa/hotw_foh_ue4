// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HotW_FoH_UE4/Enums/ECharacterEnums.h"
#include "UW_CharacterSwitch.generated.h"

class UImage;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class HOTW_FOH_UE4_API UUW_CharacterSwitch : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	
	TSoftObjectPtr<UImage> CurrentMask;	
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Character Switch UI")
	UImage* PrinceIcon;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Character Switch UI")
	UImage* GirlIcon;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Mask Switch UI")
	UImage* MaskIconA;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Mask Switch UI")
	UImage* MaskIconB;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Mask Switch UI")
	UImage* MaskIconC;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Mask Switch UI")
	UImage* MaskIconD;
	
	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim), Category = "UI Animation")
	UWidgetAnimation* SwitchToGirlAnim;
	
	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim), Category = "UI Animation")
	UWidgetAnimation* SwitchToPrinceAnim;
	
	uint8 PC_CharacterID = 0;
	
	EMaskState PC_MaskState;
	
	
	void SwitchToCharacter(int32 CharacterID);
	
	UFUNCTION(BlueprintCallable, Category="UI Functions")
	void OnSwitchCharacterAnimationFinished();
	
	void SwitchToMask(EMaskState NewMaskState);
};
