// Copyright kellendeng


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(GetWorld(),OverlayWidgetControllerClass);
		OverlayWidgetController->SetAuraWidgetController(Params);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetControllerClass,TEXT("Not Initalized OverlayWidgetController Class,Check BP_AuraHUD"));
	checkf(OverlayWidgetClass,TEXT("Not Initalized OverlayWidgetClass Class,Check BP_AuraHUD"));
	UAuraUserWidget* Widget = CreateWidget<UAuraUserWidget>(GetWorld(),OverlayWidgetClass);

	const FWidgetControllerParams Params(PC,PS,ASC,AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(Params);
	Widget->SetWidgetController(WidgetController);
	// 初始化值之后进行广播
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}
