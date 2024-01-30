// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CL_AssetManager.generated.h"

/**
 * 
 */
UCLASS()
class CANDLELIT_API UCL_AssetManager : public UAssetManager
{
	GENERATED_BODY()
public:

	static UCL_AssetManager& Get();

	virtual void StartInitialLoading() override;
};
