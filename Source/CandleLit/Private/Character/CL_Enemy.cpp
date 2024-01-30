// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CL_Enemy.h"

#include "AbilitySystemComponent.h"
#include "CL_GameplayTags.h"
#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "AbilitySystem/CL_AbilitySystemLibrary.h"
#include "AbilitySystem/CL_AttributeSet.h"
#include "AI/CL_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"


// Sets default values
ACL_Enemy::ACL_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	AbilitySystemComponent = CreateDefaultSubobject<UCL_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	AttributeSet = CreateDefaultSubobject<UCL_AttributeSet>("AttributeSet");
	// HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	// HealthBar->SetupAttachment(GetRootComponent());
}

void ACL_Enemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	
	if(!HasAuthority()) return;
	
	CL_AIController = Cast<ACL_AIController>(NewController);

	CL_AIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	CL_AIController->RunBehaviorTree(BehaviorTree);
	CL_AIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	
}

void ACL_Enemy::HighlightActor()
{
}

void ACL_Enemy::UnHighLightActor()
{
}

void ACL_Enemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	IEnemyInterface::SetCombatTarget_Implementation(InCombatTarget);
}

AActor* ACL_Enemy::GetCombatTarget_Implementation() const
{
	return IEnemyInterface::GetCombatTarget_Implementation();
}

int32 ACL_Enemy::GetPlayerLevel()
{
	return Super::GetPlayerLevel();
}

void ACL_Enemy::Die()
{
	Super::Die();
}

void ACL_Enemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
}

void ACL_Enemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
}

void ACL_Enemy::InitializeDefaultAttributes() const
{
	Super::InitializeDefaultAttributes();
}

// Called when the game starts or when spawned
void ACL_Enemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0: BaseWalkSpeed;
	InitAbilityActorInfo();
	if(HasAuthority()) UCL_AbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterType);
	
	// if(UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	// {
	// 	AuraUserWidget->SetWidgetController(this);
	// }
	//
	if(const UCL_AttributeSet* AuraAS = Cast<UCL_AttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		AbilitySystemComponent->RegisterGameplayTagEvent(FCL_GameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&ACL_Enemy::HitReactTagChanged
		);
		
		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
}

// Called every frame
void ACL_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


