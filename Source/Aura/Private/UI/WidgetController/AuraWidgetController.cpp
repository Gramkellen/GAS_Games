// Copyright kellendeng


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetAuraWidgetController(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}

void UAuraWidgetController::BroadcastIntialValues()
{
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}
