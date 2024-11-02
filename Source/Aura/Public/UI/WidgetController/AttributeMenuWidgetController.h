// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfo;
struct FAuraAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeMenuSignature, const FAuraAttributeInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;

	virtual  void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable,Category="GAS|AttributeMenu")
	FAttributeMenuSignature AttributeMenuSignature;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAttributeInfo* AttributeMenuInfo;

private:
	FORCEINLINE void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
