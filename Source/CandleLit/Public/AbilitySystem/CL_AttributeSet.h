// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CL_AttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

#define CONSTRUCT_ATTRIBUTE_REP_NOTIFy(AttributeName)\
FORCEINLINE void OnRep_##AttributeName(const FGameplayAttributeData& OldAttribute) const\
{\
GAMEPLAYATTRIBUTE_REPNOTIFY(ClassName, AttributeName, OldAttribute);\
}

#define CONSTRUCT_ATTRIBUTE(ClassName, AttributeName, PropCategory)\
FGameplayAttributeData AttributeName;\
ATTRIBUTE_ACCESSORS(ClassName, AttributeName);

USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties()
	{
	};
	UPROPERTY()
	FGameplayEffectContextHandle EffectContextHandle;


	//Source
	UPROPERTY()
	UAbilitySystemComponent* SourceASC{nullptr};
	UPROPERTY()
	AActor* SourceAvatarActor{nullptr};
	UPROPERTY()
	AController* SourceController{nullptr};
	UPROPERTY()
	ACharacter* SourceCharacter{nullptr};

	// Target
	UPROPERTY()
	UAbilitySystemComponent* TargetASC{nullptr};
	UPROPERTY()
	AActor* TargetAvatarActor{nullptr};
	UPROPERTY()
	AController* TargetController{nullptr};
	UPROPERTY()
	ACharacter* TargetCharacter{nullptr};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedDelegate, float, Amount);

// Typedef is specific to the FGameplayAttribute() signature but TStaticFuncPtr is generic to any signature
//typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

UCLASS()
class CANDLELIT_API UCL_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	UCL_AttributeSet();
	
	public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	//TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;
	
	/*
	 * Primary Attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category= "VitalAttributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCL_AttributeSet, Health);
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	FOnAttributeChangedDelegate HealthChanged;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category= "VitalAttributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCL_AttributeSet, MaxHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Oxygen, Category= "VitalAttributes")
	FGameplayAttributeData Oxygen;
	ATTRIBUTE_ACCESSORS(UCL_AttributeSet, Oxygen);
	UFUNCTION()
	void OnRep_Oxygen(const FGameplayAttributeData& OldHealth) const;
	FOnAttributeChangedDelegate OxygenChanged;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxOxygen, Category= "VitalAttributes")
	FGameplayAttributeData MaxOxygen;
	ATTRIBUTE_ACCESSORS(UCL_AttributeSet, MaxOxygen);
	UFUNCTION()
	virtual void OnRep_MaxOxygen(const FGameplayAttributeData& OldMaxOxygen);
	
private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;

	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const;
};
