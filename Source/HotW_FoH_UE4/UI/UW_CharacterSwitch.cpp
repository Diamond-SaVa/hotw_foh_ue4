// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "UW_CharacterSwitch.h"

void UUW_CharacterSwitch::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (SwitchToPrinceAnim != nullptr && SwitchToGirlAnim != nullptr)
	{
		// Declare the dynamic event
		FWidgetAnimationDynamicEvent AnimationFinishedEvent;
        
		// Bind the C++ function to the delegate
		AnimationFinishedEvent.BindDynamic(this, &UUW_CharacterSwitch::OnSwitchCharacterAnimationFinished);

		// Register the event with the User Widget
		BindToAnimationFinished(SwitchToPrinceAnim, AnimationFinishedEvent);
		BindToAnimationFinished(SwitchToGirlAnim, AnimationFinishedEvent);
	}
}

void UUW_CharacterSwitch::SwitchToCharacter(int32 CharacterID)
{
	switch (CharacterID)
	{
	case 0:
		PlayAnimation(SwitchToPrinceAnim);
		break;
	case 1:
		PlayAnimation(SwitchToGirlAnim);
		break;
	default:
		break;
	}
}

void UUW_CharacterSwitch::OnSwitchCharacterAnimationFinished()
{
	if (AGameModeBase* GMode = GetWorld()->GetAuthGameMode())
	{
		
	}
}
