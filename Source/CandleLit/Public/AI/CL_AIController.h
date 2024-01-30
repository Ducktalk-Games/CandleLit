// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CL_AIController.generated.h"


class UBehaviorTreeComponent;
class UBlackboardComponent;
UCLASS()
class CANDLELIT_API ACL_AIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACL_AIController();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

};
