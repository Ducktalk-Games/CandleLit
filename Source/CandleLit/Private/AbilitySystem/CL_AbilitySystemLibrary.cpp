// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CL_AbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/CharacterInfo.h"
#include "Game/CL_GameMode.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

void UCL_AbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                           ECharacterInfo CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	const AActor* AvatarActor = ASC->GetAvatarActor();
	
	UCharacterInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	const FCharacterDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);	
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle=  ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());

	
}

void UCL_AbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,
	ECharacterInfo CharacterClass)
{
	UCharacterInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return;
	for(const TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	
	const FCharacterDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for(TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterInfo* UCL_AbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const ACL_GameMode* CLGameMode = Cast<ACL_GameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(CLGameMode == nullptr) return nullptr;

	UCharacterInfo* CharacterInfo = CLGameMode->CharacterInfo;
	return CharacterInfo;
}

void UCL_AbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for(FOverlapResult& Overlap: Overlaps)
		{
			if(Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UCL_AbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bFirstIsPlayer = FirstActor->ActorHasTag(FName("Player"));
	const bool bSecondIsPlayer = SecondActor->ActorHasTag(FName("Player"));

	if(bFirstIsPlayer == bSecondIsPlayer) return false;
	return true;
	
}
