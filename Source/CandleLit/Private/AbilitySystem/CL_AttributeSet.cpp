// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CL_AttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CL_GameplayTags.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/CL_PlayerController.h"

UCL_AttributeSet::UCL_AttributeSet()
{
	const FCL_GameplayTags& GameplayTags = FCL_GameplayTags::Get();

	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
}

void UCL_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UCL_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCL_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCL_AttributeSet, Oxygen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCL_AttributeSet, MaxOxygen, COND_None, REPNOTIFY_Always);
}

void UCL_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
		
	}
	if(Attribute == GetOxygenAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxOxygen());
	}
}

void UCL_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
		HealthChanged.Broadcast(GetHealth());
		if(GetHealth() <= 0.f)
		{
			if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
			{
				CombatInterface->Die();
			}
		}
	}
	if(Data.EvaluatedData.Attribute == GetOxygenAttribute())
	{
		SetOxygen(FMath::Clamp(GetOxygen(), 0, GetMaxOxygen()));
		OxygenChanged.Broadcast(GetOxygen());
	}
}

void UCL_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
}

void UCL_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
}

void UCL_AttributeSet::OnRep_Oxygen(const FGameplayAttributeData& OldHealth) const
{
}

void UCL_AttributeSet::OnRep_MaxOxygen(const FGameplayAttributeData& OldMaxOxygen)
{
}

void UCL_AttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = owner fo the effect. Target - Target of the effect (owner of the AS)
	
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if(IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if(Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if(const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if(Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UCL_AttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit,
	bool bCriticalHit) const
{
	if(Props.SourceCharacter != Props.TargetCharacter)
	{
		if(ACL_PlayerController* PC = Cast<ACL_PlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
			return;
		}
		if(ACL_PlayerController* PC = Cast<ACL_PlayerController>(Props.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
		}
	}
}
