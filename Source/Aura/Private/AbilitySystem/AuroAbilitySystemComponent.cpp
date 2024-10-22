// Copyright kellendeng


#include "AbilitySystem/AuroAbilitySystemComponent.h"

void UAuroAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuroAbilitySystemComponent::EffectApplied);
}

void UAuroAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	AssetTagsDelegate.Broadcast(TagContainer);
}
