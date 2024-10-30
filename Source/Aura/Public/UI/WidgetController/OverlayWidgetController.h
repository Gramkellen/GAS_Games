// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UAuraUserWidget;
// 为了方便根据便签检索和展示信息
// 定义了表格中每一行的结构样式
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText MessageText = FText();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* Image = nullptr;
	
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeChangedDelegate, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetDelegate,FUIWidgetRow,MessageWidget);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;

	virtual void BindCallbacksToDependencies() override;

	// 创建委托实例 ，BlueprintAssignable表示委托可以在蓝图中被赋值
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeChangedDelegate OnHealthChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeChangedDelegate OnMaxHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeChangedDelegate OnManaChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeChangedDelegate OnMaxManaChangedDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|Message")
	FMessageWidgetDelegate MessageWidgetDelegate;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "GAS|MessageData")
	TObjectPtr<UDataTable>MessageWidgetDataTable;
	
	// 数据表中查找对应的Tag名字的 Row
	template<typename T>
	T* GetRowByDataTable(UDataTable* DataTable,const FGameplayTag& Tag);
};

template <typename T>
T* UOverlayWidgetController::GetRowByDataTable(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
