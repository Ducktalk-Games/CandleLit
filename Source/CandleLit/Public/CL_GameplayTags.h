// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FCL_GameplayTags
{
public:

	static const FCL_GameplayTags& Get () {return GameplayTags;}
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxOxygen;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	
	FGameplayTag Effects_HitReact;

	FGameplayTag Montage_Interaction;
	FGameplayTag Montage_Event;
	FGameplayTag Montage_Flame_Event;
	
private:
	static FCL_GameplayTags GameplayTags;
};