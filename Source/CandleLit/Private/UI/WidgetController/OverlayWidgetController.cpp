// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "AbilitySystem/CL_AttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UCL_AttributeSet* AuraAttributeSet = CastChecked<UCL_AttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnOxygenChanged.Broadcast(AuraAttributeSet->GetOxygen());
	OnMaxOxygenChanged.Broadcast(AuraAttributeSet->GetMaxOxygen());

}

void UOverlayWidgetController::BindCallBacksToDependencies()
{
	const UCL_AttributeSet* AuraAttributeSet = CastChecked<UCL_AttributeSet>(AttributeSet);

	BIND_CALLBACK_TO_ATTRIBUTE_CHANGED(Health)
	BIND_CALLBACK_TO_ATTRIBUTE_CHANGED(MaxHealth)
	BIND_CALLBACK_TO_ATTRIBUTE_CHANGED(Oxygen)
	BIND_CALLBACK_TO_ATTRIBUTE_CHANGED(MaxOxygen)

	Cast<UCL_AbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if(Tag.MatchesTag(MessageTag) && MessageWidgetDataTable != nullptr)
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}	
	);
}
