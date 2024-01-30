// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/CL_WidgetController.h"

#include "AbilitySystem/CL_AttributeSet.h"

void UCL_WidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UCL_WidgetController::BroadcastInitialValues()
{
}

void UCL_WidgetController::BindCallBacksToDependencies()
{
	
}
