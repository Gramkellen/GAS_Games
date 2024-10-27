// Copyright kellendeng


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/AuraGameplayTags.h"

// 设置应用的Info
void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::EffectApplied);
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	GEngine->AddOnScreenDebugMessage(1,3.f,FColor::Blue,GameplayTags.Attributes_Secondary_Armor.ToString());
}

// 当有GE应用到所在的AbilitySystemCOmponent时，回调EffectApplied,然后广播这个GE对应的标签
void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	AssetTagsDelegate.Broadcast(TagContainer);
}
