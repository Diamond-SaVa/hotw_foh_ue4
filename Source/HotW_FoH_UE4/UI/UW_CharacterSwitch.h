// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="UI")
	UImage* PrinceIcon;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="UI")
	UImage* GirlIcon;
	
	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim), Category = "UI Animation")
	UWidgetAnimation* SwitchToGirlAnim;
	
	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim), Category = "UI Animation")
	UWidgetAnimation* SwitchToPrinceAnim;
	
	void SwitchToCharacter(int32 CharacterID);
	
	UFUNCTION(BlueprintCallable, Category="UI Functions")
	void OnSwitchCharacterAnimationFinished();
};
