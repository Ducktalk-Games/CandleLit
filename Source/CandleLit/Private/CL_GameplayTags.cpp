// Fill out your copyright notice in the Description page of Project Settings.


#include "CL_GameplayTags.h"

#include "GameplayTagsManager.h"

FCL_GameplayTags FCL_GameplayTags::GameplayTags;

void FCL_GameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString(""));

	// Input Tags
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag For left mouse button"));

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag For Right mouse button"));

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag For 1 key"));

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag For 2 key"));

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag For 3 key"));

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input Tag For 4 key"));
	
	/*
	* Effects
	*/
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Tag granted when hit reacting"));

	
	/*
	 * Montages
	 */

	GameplayTags.Montage_Event = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Event"),
		FString("Character"));
	GameplayTags.Montage_Interaction = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Interaction"),
		FString("Character"));
	GameplayTags.Montage_Interaction = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Flame.Event"),
		FString("Character"));
	
}
