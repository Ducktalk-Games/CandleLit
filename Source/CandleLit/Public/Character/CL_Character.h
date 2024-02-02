// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CandleLitCharacterBase.h"
#include "GameFramework/Character.h"
#include "CL_Character.generated.h"

UCLASS()
class CANDLELIT_API ACL_Character : public ACandleLitCharacterBase
{
	GENERATED_BODY()

public:
	ACL_Character();

	/*Combat interface*/
	virtual int32 GetPlayerLevel() override;
protected:
	virtual void BeginPlay() override;

	void HealthChanged(float NewValue);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_RespawnCharacter();
	
public:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	virtual void InitAbilityActorInfo() override;
	
};
