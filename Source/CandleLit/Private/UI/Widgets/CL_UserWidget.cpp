// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/CL_UserWidget.h"

void UCL_UserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
