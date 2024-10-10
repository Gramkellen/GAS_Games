// Copyright kellendeng


#include "UI/Widget/AuroUserWidget.h"

void UAuroUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
