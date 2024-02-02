// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CandleLitCharacterBase.h"
#include "AbilitySystem/CharacterInfo.h"
#include "GameFramework/Character.h"
#include "Interfaces/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "CL_Enemy.generated.h"


class UBehaviorTree;
class UWidgetComponent;
class ACL_AIController;



UCLASS()
class CANDLELIT_API ACL_Enemy : public ACandleLitCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	ACL_Enemy();

	virtual void PossessedBy(AController* NewController) override;
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

#pragma region EnemyInterface
	/*Enemy Interface*/
	
	virtual void HighlightActor() override;
	virtual void UnHighLightActor() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	
#pragma endregion EnemyInterface

#pragma region CombatInterface
	/*
	 * Combat Interface
	 */

	virtual int32 GetPlayerLevel() override;

	virtual void Die() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

#pragma endregion CombatInterface

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bExtinguished = true;
	UFUNCTION(BlueprintCallable, Category=  "Combat")
	void SetIsExtinguished(const bool Extinguished);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 400.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;
	
protected:
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level{1};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterInfo CharacterType = ECharacterInfo::Candle;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	TObjectPtr<ACL_AIController> CL_AIController;
};
