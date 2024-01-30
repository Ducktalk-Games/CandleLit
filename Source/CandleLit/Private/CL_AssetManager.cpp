// Fill out your copyright notice in the Description page of Project Settings.


#include "CL_AssetManager.h"

#include "AbilitySystemGlobals.h"
#include "CL_GameplayTags.h"

UCL_AssetManager& UCL_AssetManager::Get()
{
	check(GEngine);
	return *Cast<UCL_AssetManager>(GEngine->AssetManager);
}

void UCL_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FCL_GameplayTags::InitializeNativeGameplayTags();

	// This is required to use TargetData.
	UAbilitySystemGlobals::Get().InitGlobalData();
}
