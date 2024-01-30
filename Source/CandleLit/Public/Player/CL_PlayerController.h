// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "CL_PlayerController.generated.h"

class UDamageTextComponent;
class USplineComponent;
class UCL_InputConfig;
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UCL_AbilitySystemComponent;

UCLASS()
class CANDLELIT_API ACL_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACL_PlayerController();
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount,class ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	
protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> CandleLitContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction>  MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	bool bShiftKeyDown = false;
	void ShiftPressed() {bShiftKeyDown = true; }
	void ShiftReleased() {bShiftKeyDown = false; };

	void Move(const FInputActionValue& InputActionValue);
	void AutoRun();	

	void CursorTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	FHitResult CursorHit;
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category ="Input")
	TObjectPtr<UCL_InputConfig> InputConfig;
	
	UPROPERTY()
	TObjectPtr<UCL_AbilitySystemComponent> CL_AbilitySystemComponent;

	UCL_AbilitySystemComponent* GetASC();

	FVector CachedDestination {FVector::ZeroVector};
	float FollowTime {0.f};
	float ShortPressThreshold {0.5f};
	bool bAutoRunning {false};
	bool bTargeting {false};
	
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius {50.f};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	// UPROPERTY(EditDefaultsOnly, Category="Damage")
	// TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

};
