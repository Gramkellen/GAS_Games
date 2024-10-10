// Copyright kellendeng


#include "UI/HUD/AuroHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuroUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuroHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(GetWorld(),OverlayWidgetControllerClass);
		OverlayWidgetController->SetAuroWidgetController(Params);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void AAuroHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetControllerClass,TEXT("Not Initalized OverlayWidgetController Class,Check BP_AuroHUD"));
	checkf(OverlayWidgetClass,TEXT("Not Initalized OverlayWidgetClass Class,Check BP_AuroHUD"));
	UAuroUserWidget* Widget = CreateWidget<UAuroUserWidget>(GetWorld(),OverlayWidgetClass);

	const FWidgetControllerParams Params(PC,PS,ASC,AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(Params);
	Widget->SetWidgetController(WidgetController);
	// 初始化值之后进行广播
	WidgetController->BroadcastIntialValues();
	Widget->AddToViewport();
}
