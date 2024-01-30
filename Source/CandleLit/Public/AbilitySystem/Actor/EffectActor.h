// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"


class UAbilitySystemComponent;
UENUM(BlueprintType)
enum class EEffectApplicationPolicy: uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy: uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

class UGameplayEffect;

UCLASS()
class CANDLELIT_API AEffectActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Applied Effects")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bApplyEffectsToEnemies = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> InstantGameplayEffectClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta=(EditCondition = "InstantGameplayEffectClasses.Num() != 0"))
	EEffectApplicationPolicy InstantEffectApplicationPolicy {EEffectApplicationPolicy::DoNotApply};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> DurationGameplayEffectClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta=(EditCondition = "DurationGameplayEffectClasses.Num() != 0"))
	EEffectApplicationPolicy DurationEffectApplicationPolicy {EEffectApplicationPolicy::DoNotApply};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> InfiniteGameplayEffectClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta=(EditCondition = "InfiniteGameplayEffectClasses.Num() != 0"))
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy {EEffectApplicationPolicy::DoNotApply};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta=(EditCondition = "InfiniteGameplayEffectClasses.Num() != 0"))
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy {EEffectRemovalPolicy::RemoveOnEndOverlap};

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta=(EditCondition = "InfiniteGameplayEffectClasses.Num() != 0"))
	float ActorLevel {1.f};
	
};
