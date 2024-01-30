// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CL_PlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "AbilitySystem/CL_AttributeSet.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ACL_PlayerState::ACL_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCL_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UCL_AttributeSet>("AttributeSet");
	NetUpdateFrequency = 100.f;
}

void ACL_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACL_PlayerState, Level);
}

UAbilitySystemComponent* ACL_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ACL_PlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void ACL_PlayerState::OnRep_Level()
{
}

// Called when the game starts or when spawned
void ACL_PlayerState::BeginPlay()
{
	Super::BeginPlay();
	
}

