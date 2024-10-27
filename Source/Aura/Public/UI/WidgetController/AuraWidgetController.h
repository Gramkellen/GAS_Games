// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"


// 表示可以在蓝图中进行使用，该结构体主要用于初始化使用
USTRUCT(BlueprintType)
struct  FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams(){};
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS):
	PlayerController(PC),PlayerState(PS),AbilitySystemComponent(ASC),AttributeSet(AS){};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController>PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState>PlayerState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet>AttributeSet;
	
};
/**
 * 
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	void SetAuraWidgetController(const FWidgetControllerParams& Params);

	virtual void BroadcastIntialValues();

	virtual void BindCallbacksToDependencies();
protected:
	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<APlayerController>PlayerController;

	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<APlayerState>PlayerState;

	// 这里为什么不用Aura的？
	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly,Category = "WidgetController")
	TObjectPtr<UAttributeSet>AttributeSet;
};
