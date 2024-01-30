// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CL_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class CANDLELIT_API UCL_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category ="Input")
	FGameplayTag StartupInputTag;

};
