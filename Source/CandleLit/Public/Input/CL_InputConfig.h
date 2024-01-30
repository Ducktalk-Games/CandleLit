// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CL_InputConfig.generated.h"

USTRUCT(BlueprintType)
struct FCLInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction {nullptr};

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag {FGameplayTag()};
};
/**
 * 
 */
UCLASS()
class CANDLELIT_API UCL_InputConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCLInputAction> AbilityInputActions;
};
