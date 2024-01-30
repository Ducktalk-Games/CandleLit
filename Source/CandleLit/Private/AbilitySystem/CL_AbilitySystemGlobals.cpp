// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CL_AbilitySystemGlobals.h"

#include "AbilitySystem/CL_AbilityTypes.h"

FGameplayEffectContext* UCL_AbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return  new FCL_GameplayEffectContext();
}
