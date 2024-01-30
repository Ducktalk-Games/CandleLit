// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Actor/EffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"


// Sets default values
AEffectActor::AEffectActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Scene Root"));
}

// Called when the game starts or when spawned
void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{

	if(TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return; 
	
	checkf(GameplayEffectClass, TEXT("Effect actor Gameplay effect class is not set!"));
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if(bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}

	if(bDestroyOnEffectApplication && EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy != EGameplayEffectDurationType::Infinite)
	{
		Destroy();
	}
}

void AEffectActor::OnOverlap(AActor* TargetActor)
{
	if(TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (const auto InstantGameplayEffectClass: InstantGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
		}
	}

	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (const auto DurationGameplayEffectClass: DurationGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
		}
	}

	if(InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (const auto InfiniteGameplayEffectClass: InfiniteGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
		}
	}
}

void AEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if(TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const auto InstantGameplayEffectClass: InstantGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
		}
	}
	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const auto DurationGameplayEffectClass: DurationGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
		}
	}
	if(InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const auto InfiniteGameplayEffectClass: InfiniteGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
			
		}
	}
	if(InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if(!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for(const auto HandlePair : ActiveEffectHandles)
		{
			if(TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		for(auto Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}


