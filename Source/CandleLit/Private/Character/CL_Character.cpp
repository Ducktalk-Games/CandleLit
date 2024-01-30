// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CL_Character.h"

#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/CL_PlayerController.h"
#include "Player/CL_PlayerState.h"
#include "UI/HUD/CL_HUD.h"


// Sets default values
ACL_Character::ACL_Character()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

int32 ACL_Character::GetPlayerLevel()
{
	return 1;
}

void ACL_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init Ability Actor Info For the server.
	InitAbilityActorInfo();

	AddCharacterAbilities();
}

void ACL_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Init Ability Actor info for the client.
	InitAbilityActorInfo();
}

void ACL_Character::InitAbilityActorInfo()
{
	ACL_PlayerState* CLPlayerState = GetPlayerState<ACL_PlayerState>();
	check(CLPlayerState);

	CLPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(CLPlayerState, this);
	Cast<UCL_AbilitySystemComponent>(CLPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = CLPlayerState->GetAbilitySystemComponent();
	AttributeSet = CLPlayerState->GetAttributeSet();

	if(ACL_PlayerController* CLPlayerController = Cast<ACL_PlayerController>(GetController()))
	{
		if(ACL_HUD* CLHUD = Cast<ACL_HUD>(CLPlayerController->GetHUD()))
		{
			CLHUD->InitOverlay(CLPlayerController, CLPlayerState,AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
}

// Called when the game starts or when spawned
void ACL_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACL_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACL_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

