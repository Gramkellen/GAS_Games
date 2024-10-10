// Copyright kellendeng


#include "UI/WidgetController/AuroWidgetController.h"

void UAuroWidgetController::SetAuroWidgetController(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}

void UAuroWidgetController::BroadcastIntialValues()
{
}

void UAuroWidgetController::BindCallbacksToDependencies()
{
}
