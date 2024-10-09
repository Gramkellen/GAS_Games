// Copyright kellendeng


#include "UI/Widget/AuroUserWidget.h"

void UAuroUserWidget::SetWidgetControler(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
