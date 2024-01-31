// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CameraVolume.h"

#include "CandleLitCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Character/CL_Character.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


ACameraVolume::ACameraVolume(const FObjectInitializer& ObjectInitialier)
	: Super(ObjectInitialier)
{
	PrimaryActorTick.bCanEverTick = true;

	// ActiveZone = CreateDefaultSubobject<UBoxComponent>("ActiveZone");
	// ActiveZone->SetBoxExtent(FVector(100.f,100.f,50.f));
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RailCameraMount);
	
}

void ACameraVolume::BeginPlay()
{
	Super::BeginPlay();

	Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	InitialCameraTransform = Camera->GetComponentTransform();
	
}

void ACameraVolume::OnOverlap(AActor* TargetActor)
{
	Character = Cast<ACandleLitCharacterBase>(TargetActor);
	if(Character)
	{
		Character->SetCurrentCameraVolume(this);
		if(Controller)
		{
			Controller->SetViewTargetWithBlend(this, CameraBlendSpeed, BlendFunction, BlendExp, bBlockOutGoing);
		}
	}
}

void ACameraVolume::OnEndOverlap(AActor* TargetActor)
{
	Character = Cast<ACandleLitCharacterBase>(TargetActor);
	if(Character && Character->GetCurrentCameraVolume() == this)
	{
		Character->SetCurrentCameraVolume(nullptr);
		if(Controller)
		{
			Controller->SetViewTargetWithBlend(Character, CameraBlendSpeed, BlendFunction, BlendExp, bBlockOutGoing);
		}
	}
}

void ACameraVolume::FollowCharacterOnSpline(float DeltaTime)
{
	if(Character)
	{
		const FVector CameraLocation = Camera->GetComponentLocation();
		const FVector CharacterLocation = Character->GetActorLocation();

		const FVector OffsetPosition = CharacterLocation + Character->GetActorForwardVector() * Offset;
		

		// Calculate rotation to look at the offset position
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, CharacterLocation);
		const FRotator CameraRotation = Camera->GetComponentRotation();
		const FRotator InterpToRotation = FMath::RInterpTo(CameraRotation, LookAtRotation, DeltaTime, RotationInterpSpeed);

		// Find the closest point on the spline to the offset position
		const FVector LocationClosestToWorldLocation = GetRailSplineComponent()->FindLocationClosestToWorldLocation(CharacterLocation + FVector(-MaxFollowDistance, 0, MaxFollowDistance), ESplineCoordinateSpace::World);

		// Push Camera back


		const FVector IdealPosition = LocationClosestToWorldLocation ;
		
		// Interpolate camera location towards the closest point on the spline
		const FVector InterpToLocation = FMath::VInterpTo(CameraLocation, IdealPosition, DeltaTime, CameraMovementSpeed);

		// 
		// Set the camera location and rotation
		Camera->SetWorldLocationAndRotation(InterpToLocation, InterpToRotation);
	}
}



void ACameraVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bUseSpline)
	{
		FollowCharacterOnSpline(DeltaTime);
	}
}

