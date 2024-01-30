// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CL_AIController.h"


// Sets default values
ACL_AIController::ACL_AIController()
{
	PrimaryActorTick.bCanEverTick = false;

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard Component");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviourTreeComponent");
	check(BehaviorTreeComponent);
}
