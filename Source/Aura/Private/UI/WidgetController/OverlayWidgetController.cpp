// Copyright kellendeng


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuroAttributeSet.h"

// 设置初始化的值并进行广播，委托的事件写在了蓝图里面
void UOverlayWidgetController::BroadcastIntialValues()
{
	if(UAuroAttributeSet* AuroAttribute = Cast<UAuroAttributeSet>(AttributeSet))
	{
		OnHealthChangedDelegate.Broadcast(AuroAttribute->GetHealth());
		OnMaxHealthChangedDelegate.Broadcast(AuroAttribute->GetMaxHealth());
		OnManaChangedDelegate.Broadcast(AuroAttribute->GetMana());
		OnMaxManaChangedDelegate.Broadcast(AuroAttribute->GetMaxMana());
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// UE自带的属性改变委托
	const UAuroAttributeSet *AuroAttributeSet = Cast<UAuroAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuroAttributeSet->GetHealthAttribute())
	                       .AddUObject(this,&UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuroAttributeSet->GetMaxHealthAttribute())
						   .AddUObject(this,&UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuroAttributeSet->GetManaAttribute())
	                       .AddUObject(this, &UOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuroAttributeSet->GetMaxManaAttribute())
	                       .AddUObject(this,&UOverlayWidgetController::MaxManaChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChangedDelegate.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChangedDelegate.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChangedDelegate.Broadcast(Data.NewValue);
}
