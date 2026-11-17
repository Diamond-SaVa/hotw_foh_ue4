// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "UW_CharacterSwitch.h"

#include "Components/Image.h"

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
	PC_CharacterID = CharacterID;
	
	switch (CharacterID)
	{
	case 0:
		
		PlayAnimation(SwitchToPrinceAnim);
		
		PrinceIcon->SetColorAndOpacity(FLinearColor::White);
		
		GirlIcon->SetColorAndOpacity(FLinearColor::Gray);
		
		break;
	case 1:
		
		PlayAnimation(SwitchToGirlAnim);
		
		PrinceIcon->SetColorAndOpacity(FLinearColor::Gray);
		
		GirlIcon->SetColorAndOpacity(FLinearColor::White);
		
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

void UUW_CharacterSwitch::SwitchToMask(const EMaskState NewMaskState)
{
	if (PC_MaskState == NewMaskState)
	{
		PC_MaskState = EMaskState::EM_None;
	}
	else
	{
		PC_MaskState = NewMaskState;
	}
	
	const FVector2D OneSize = FVector2D::UnitVector;
	
	const FVector2D TwoSize = OneSize * 1.5f;
	
	if (CurrentMask.IsValid())
	{
		CurrentMask.Get()->SetRenderScale(OneSize);
	}
	
	switch (PC_MaskState)
	{
	case EMaskState::EM_Angel:
		CurrentMask = MaskIconA;
		MaskIconA->SetRenderScale(TwoSize);
		
		break;
	case EMaskState::EM_Ninja:
		CurrentMask = MaskIconB;
		MaskIconB->SetRenderScale(TwoSize);
		
		break;
	case EMaskState::EM_Diver:
		CurrentMask = MaskIconC;
		MaskIconC->SetRenderScale(TwoSize);
		
		break;
		
	case EMaskState::EM_Winds:
		CurrentMask = MaskIconD;
		MaskIconD->SetRenderScale(TwoSize);
		
		break;
		
	case EMaskState::EM_None:
		break;
	}
}
