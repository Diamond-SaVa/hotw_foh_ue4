// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_CharacterSwitch.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class HOTW_FOH_UE4_API UUW_CharacterSwitch : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="UI")
	UImage* PrinceIcon;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="UI")
	UImage* GirlIcon;
};
