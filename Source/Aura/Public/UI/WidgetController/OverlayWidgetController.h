// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuroWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UAuroUserWidget;
// 为了方便根据便签检索和展示信息
// 定义了表格中每一行的结构样式
USTRUCT(BlueprintType)
struct FUIWigetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText MessageText = FText();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UAuroUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* Image = nullptr;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChanged, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChanged, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChanged, float, NewMaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetDelegate,FUIWigetRow,MessageWidget);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuroWidgetController
{
	GENERATED_BODY()
public:
	void BroadcastIntialValues() override;

	void BindCallbacksToDependencies() override;

	void HealthChanged(const FOnAttributeChangeData& Data) const;

	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;

	void ManaChanged(const FOnAttributeChangeData& Data) const;

	void MaxManaChanged(const FOnAttributeChangeData& Data) const;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Message")
	TObjectPtr<UDataTable>MessageWidgetDataTable;
	
	// 创建委托实例 ，BlueprintAssignable表示委托可以在蓝图中被赋值
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnHealthChanged OnHealthChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxHealthChanged OnMaxHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnManaChanged OnManaChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnManaChanged OnMaxManaChangedDelegate;

	template<typename T>
	T* GetRowByDataTable(UDataTable* Table,const FGameplayTag& Tag);

	UPROPERTY(BlueprintCallable,Category="GAS|Message")
	FMessageWidgetDelegate MessageWidgetDelegate;
};