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

	Cast<UAuroAbilitySystemComponent>(AbilitySystemComponent)->AssetTagsDelegate.AddLambda(
		[this](const FGameplayTagContainer& TagContainer)->void
		{
			for(const FGameplayTag &Tag : TagContainer)
			{
				FString Msg = FString::Printf(TEXT("Game play Tag : %s"),*Tag.ToString());
				GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Yellow,Msg);
				// Lambda访问的时候需要进行捕获
				FUIWigetRow* Row = GetRowByDataTable<FUIWigetRow>(MessageWidgetDataTable,Tag);
			}
		}
	);
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
