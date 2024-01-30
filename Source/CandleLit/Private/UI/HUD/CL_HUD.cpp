// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/CL_HUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/CL_WidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widgets/CL_UserWidget.h"


UOverlayWidgetController* ACL_HUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallBacksToDependencies();
	}
	return OverlayWidgetController;
}

// UAttributeMenuWidgetController* ACL_HUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
// {
// 	// if(AttributeMenuWidgetController == nullptr)
// 	// {
// 	// 	AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
// 	// 	AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
// 	// 	AttributeMenuWidgetController->BindCallBacksToDependencies();
// 	// }
// 	// return AttributeMenuWidgetController;
// 	return nullptr;
// }

void ACL_HUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget class uninitialized, please fill out BP_CandleLitHUD"))
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget class uninitialized, please fill out BP_CandleLitHUD"))
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UCL_UserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS,ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}
