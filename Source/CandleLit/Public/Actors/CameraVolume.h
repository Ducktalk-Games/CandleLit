// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraRig_Rail.h"
#include "CameraVolume.generated.h"

class ACandleLitCharacterBase;
class UCameraComponent;
UCLASS()
class CANDLELIT_API ACameraVolume : public ACameraRig_Rail
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACameraVolume(const FObjectInitializer& ObjectInitialier);
	virtual void Tick(float DeltaTime) override;


	virtual void BeginPlay() override;
	

	// UPROPERTY(EditAnywhere, Category = "Zone")
	// TObjectPtr<UBoxComponent> ActiveZone;

protected:
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	
	
	UPROPERTY(EditAnywhere, Category ="Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category="Zone")
	bool bUseSpline;
	UPROPERTY(EditAnywhere, Category="Zone")
	float RotationInterpSpeed = 2.f;
	UPROPERTY(EditAnywhere, Category="Zone")
	float CameraMovementSpeed = 2.f;
	UPROPERTY(EditAnywhere, Category = "Zone")
	float CameraBlendSpeed= 2.f;
	UPROPERTY(EditAnywhere, Category = "Zone")
	TEnumAsByte<EViewTargetBlendFunction> BlendFunction{EViewTargetBlendFunction::VTBlend_Linear};
	UPROPERTY(EditAnywhere, Category = "Zone")
	float BlendExp = 0.f;
	UPROPERTY(EditAnywhere, Category = "Zone")
	bool bBlockOutGoing = false;
	UPROPERTY(EditAnywhere, Category = "Zone")
	FVector Offset {FVector()};
	UPROPERTY(EditAnywhere, Category = "Zone")
	float MaxFollowDistance = 900.f;
	
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void FollowCharacterOnSpline(float DeltaTime);
private:
	
	APlayerController* Controller;
	
	TObjectPtr<ACandleLitCharacterBase> Character;
	FTransform InitialCameraTransform;
};
