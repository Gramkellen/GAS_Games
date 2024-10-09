// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuroUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuroUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControler(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject>WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
};
