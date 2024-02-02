// Fill out your copyright notice in the Description page of Project Settings.


#include "CandleLitCharacterBase.h"

#include "CL_GameplayTags.h"
#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "AbilitySystem/CL_AttributeSet.h"
#include "Components/CapsuleComponent.h"


ACandleLitCharacterBase::ACandleLitCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Flame = CreateDefaultSubobject<USkeletalMeshComponent>("Flame");
	Flame->SetupAttachment(GetMesh(), FName("FlameTipLocation"));
	Flame->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACandleLitCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	CastChecked<UCL_AttributeSet>(GetAttributeSet())->OxygenChanged.AddDynamic(this, &ThisClass::OnOxygenChanged);
}

UAbilitySystemComponent* ACandleLitCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* ACandleLitCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

FVector ACandleLitCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FCL_GameplayTags GameplayTags = FCL_GameplayTags::Get();
	if(MontageTag.MatchesTagExact(GameplayTags.Montage_Flame_Event) && IsValid(Flame))
	{
		return Flame->GetSocketLocation(CandleTipSocketName);
	}
	return FVector();
}


void ACandleLitCharacterBase::Die()
{
	Flame->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
	bDead = true;
}

bool ACandleLitCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* ACandleLitCharacterBase::GetAvatar_Implementation()
{
	return this;
}


void ACandleLitCharacterBase::MulticastHandleDeath_Implementation()
{
	Flame->SetSimulatePhysics(true);
	Flame->SetEnableGravity(true);
	Flame->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
}

TArray<FTaggedMontage> ACandleLitCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

void ACandleLitCharacterBase::OnOxygenChanged_Implementation(float Amount)
{
}

void ACandleLitCharacterBase::InitAbilityActorInfo()
{
}

void ACandleLitCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(IsValid(GameplayEffectClass));
	
	FGameplayEffectContextHandle ContextHandle= GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, 1.f, ContextHandle);
	
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
	
}

void ACandleLitCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes);
	ApplyEffectToSelf(DefaultVitalAttributes);
}

void ACandleLitCharacterBase::AddCharacterAbilities() const
{
	UCL_AbilitySystemComponent* CL_ASC = CastChecked<UCL_AbilitySystemComponent>(AbilitySystemComponent);
	if(!HasAuthority()) return;

	CL_ASC->AddCharacterAbilities(StartupAbilities);
	
}

void ACandleLitCharacterBase::Dissolve()
{
	if(IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);

		StartDissolveTimeline(DynamicMatInst);
	}
	if(IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Flame->SetMaterial(0, DynamicMatInst);

		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}

void ACandleLitCharacterBase::SetCurrentCameraVolume(ACameraVolume* NewZone)
{
	CurrentCameraVolume = NewZone;
}
