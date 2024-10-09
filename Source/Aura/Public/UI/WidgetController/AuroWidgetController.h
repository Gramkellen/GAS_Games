// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuroAbilitySystemComponent.h"
#include "UObject/NoExportTypes.h"
#include "AuroWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuroWidgetController : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<APlayerController>PlayerController;

	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<APlayerState>PlayerState;

	// 这里为什么不用Auro的？
	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<UAttributeSet>Attribute;
};
