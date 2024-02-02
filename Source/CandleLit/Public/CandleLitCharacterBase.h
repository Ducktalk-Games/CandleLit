// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"
#include "Actors/CameraVolume.h"
#include "CandleLitCharacterBase.generated.h"


class UNiagaraSystem;
class UAnimMontage;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;

UCLASS(Abstract)
class CANDLELIT_API ACandleLitCharacterBase : public ACharacter , public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACandleLitCharacterBase();

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

	/* Combat interface */
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	
	virtual void Die() override;

	virtual bool IsDead_Implementation() const override;

	virtual AActor* GetAvatar_Implementation()  override;
	
	/* Combat interface */
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
protected:

	bool bDead = false;

	UFUNCTION(BlueprintNativeEvent, Category = "Oxygen")
	void OnOxygenChanged(float Amount);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Combat")
	TObjectPtr<USkeletalMeshComponent> Flame;

	UPROPERTY(EditAnywhere, Category ="Combat")
	FName CandleTipSocketName;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level = 1.f) const;
	
	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilities() const;

	/* Dissolve Effects*/

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

public:
	UFUNCTION(BlueprintCallable, Category ="Camera")
	void SetCurrentCameraVolume(ACameraVolume* NewZone);
	UFUNCTION(BlueprintCallable, Category ="Camera")
	ACameraVolume* GetCurrentCameraVolume() {return CurrentCameraVolume; }

	UPROPERTY()	
	TObjectPtr<ACameraVolume> CurrentCameraVolume;

private:
	UPROPERTY(EditAnywhere, Category ="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category ="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;



};
