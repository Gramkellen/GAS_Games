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
	                       .AddLambda(
	                       	[this](const FOnAttributeChangeData& Data)->void
	                       	{
	                       		OnHealthChangedDelegate.Broadcast(Data.NewValue);
	                       	});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuroAttributeSet->GetMaxHealthAttribute())
	                      .AddLambda(
		                      [this](const FOnAttributeChangeData& Data)-> void
		                      {
			                      OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
		                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuroAttributeSet->GetManaAttribute())
	                      .AddLambda(
		                      [this](const FOnAttributeChangeData& Data)-> void
		                      {
			                      OnManaChangedDelegate.Broadcast(Data.NewValue);
		                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuroAttributeSet->GetMaxManaAttribute())
	                      .AddLambda(
		                      [this](const FOnAttributeChangeData& Data)-> void
		                      {
			                      OnMaxManaChangedDelegate.Broadcast(Data.NewValue);
		                      });

	Cast<UAuroAbilitySystemComponent>(AbilitySystemComponent)->AssetTagsDelegate.AddLambda(
		[this](const FGameplayTagContainer& TagContainer)->void
		{
			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			for(const FGameplayTag &Tag : TagContainer)
			{
				if(Tag.MatchesTag(MessageTag))
				{
					if(FUIWigetRow* Row = GetRowByDataTable<FUIWigetRow>(MessageWidgetDataTable,Tag))
					{
						MessageWidgetDelegate.Broadcast(*Row);
					}
				}
			}
		}
	);
}
