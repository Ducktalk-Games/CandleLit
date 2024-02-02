// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CL_PlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CL_GameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/CL_InputComponent.h"
#include "Interfaces/EnemyInterface.h"


// Sets default values
ACL_PlayerController::ACL_PlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void ACL_PlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter,
	bool bBlockedHit, bool bCriticalHit)
{
	// if(IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	// {
	// 	UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
	// 	DamageText->RegisterComponent();
	// 	DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	// 	DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// 	DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	// }
}

void ACL_PlayerController::JumpPressed()
{
	bJumpKeyDown = true;
	if(IsValid(GetCharacter())) GetCharacter()->Jump();
}


void ACL_PlayerController::JumpReleased()
{
	bJumpKeyDown = true;
	if(IsValid(GetCharacter())) GetCharacter()->StopJumping();
}


void ACL_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if(UCL_InputComponent* CandleInputComponent = Cast<UCL_InputComponent>(InputComponent))
	{
		CandleInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACL_PlayerController::Move);
		CandleInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACL_PlayerController::JumpPressed);
		CandleInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACL_PlayerController::JumpReleased);
		CandleInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ACL_PlayerController::ShiftPressed);
		CandleInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ACL_PlayerController::ShiftReleased);
		CandleInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
}

void ACL_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	// AutoRun();
}

void ACL_PlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = PlayerCameraManager->GetCameraRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ACL_PlayerController::AutoRun()
{
	if(!bAutoRunning) return;
	if(APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if(DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void ACL_PlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	
	if(LastActor != ThisActor)
	{
		if(LastActor) LastActor->UnHighLightActor();
		if(ThisActor) ThisActor->HighlightActor();
	}
}

void ACL_PlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(InputTag.MatchesTagExact(FCL_GameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void ACL_PlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(!InputTag.MatchesTagExact(FCL_GameplayTags::Get().InputTag_LMB))
	{
		if(GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	if(GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	
	if(!bTargeting && !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn();
		if(FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if(UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,
				ControlledPawn->GetActorLocation(),
				CachedDestination
				))
			{
				Spline->ClearSplinePoints();
				for(FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					
				}
				if(NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() -1];
					bAutoRunning = true;	
				}
				
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void ACL_PlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if(!InputTag.MatchesTagExact(FCL_GameplayTags::Get().InputTag_LMB))
	{
		if(GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}
	
	if(bTargeting || bShiftKeyDown)
	{
		if(GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		if(CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;

		if(APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UCL_AbilitySystemComponent* ACL_PlayerController::GetASC()
{
	if(CL_AbilitySystemComponent == nullptr)
	{
		CL_AbilitySystemComponent = Cast<UCL_AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return CL_AbilitySystemComponent;	
}

// Called when the game starts or when spawned
void ACL_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(CandleLitContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(CandleLitContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

	GetASC();
	
}

