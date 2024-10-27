// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UAuraWidgetController;
class UAuraUserWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget>OverlayWidget;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController>OverlayWidgetController;

	UFUNCTION(BlueprintCallable)
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& Params);

	UFUNCTION()
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent * ASC, UAttributeSet * AS);
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget>OverlayWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraWidgetController>OverlayWidgetControllerClass;
};
