// Copyright kellendeng


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"

// 设置初始化的值并进行广播，委托的事件写在了蓝图里面
void UOverlayWidgetController::BroadcastInitialValues()
{
	if(UAuraAttributeSet* AuraAttribute = Cast<UAuraAttributeSet>(AttributeSet))
	{
		OnHealthChangedDelegate.Broadcast(AuraAttribute->GetHealth());
		OnMaxHealthChangedDelegate.Broadcast(AuraAttribute->GetMaxHealth());
		OnManaChangedDelegate.Broadcast(AuraAttribute->GetMana());
		OnMaxManaChangedDelegate.Broadcast(AuraAttribute->GetMaxMana());
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// UE自带的属性改变委托
	const UAuraAttributeSet *AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
	                       .AddLambda(
	                       	[this](const FOnAttributeChangeData& Data)->void
	                       	{
	                       		OnHealthChangedDelegate.Broadcast(Data.NewValue);
	                       	});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
	                      .AddLambda(
		                      [this](const FOnAttributeChangeData& Data)-> void
		                      {
			                      OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
		                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
	                      .AddLambda(
		                      [this](const FOnAttributeChangeData& Data)-> void
		                      {
			                      OnManaChangedDelegate.Broadcast(Data.NewValue);
		                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
	                      .AddLambda(
		                      [this](const FOnAttributeChangeData& Data)-> void
		                      {
			                      OnMaxManaChangedDelegate.Broadcast(Data.NewValue);
		                      });

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AssetTagsDelegate.AddLambda(
		[this](const FGameplayTagContainer& TagContainer)->void
		{
			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			for(const FGameplayTag &Tag : TagContainer)
			{
				if(Tag.MatchesTag(MessageTag))
				{
					if(FUIWidgetRow* Row = GetRowByDataTable<FUIWidgetRow>(MessageWidgetDataTable,Tag))
					{
						MessageWidgetDelegate.Broadcast(*Row);
					}
				}
			}
		}
	);
}
