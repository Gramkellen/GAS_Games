// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuroHUD.generated.h"

struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UAuroWidgetController;
class UAuroUserWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuroHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UAuroUserWidget>OverlayWidget;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController>OverlayWidgetController;

	UFUNCTION(BlueprintCallable)
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& Params);

	UFUNCTION()
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent * ASC, UAttributeSet * AS);
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuroUserWidget>OverlayWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuroWidgetController>OverlayWidgetControllerClass;
};
