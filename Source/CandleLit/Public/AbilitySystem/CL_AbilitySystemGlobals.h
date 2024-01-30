// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "UObject/Object.h"
#include "CL_AbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class CANDLELIT_API UCL_AbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
