// Copyright kellendeng


#include "AbilitySystem/AuroAbilitySystemComponent.h"

void UAuroAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuroAbilitySystemComponent::EffectApplied);
}

void UAuroAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	GEngine->AddOnScreenDebugMessage(1,8.f,FColor::Yellow,FString("Effect Applied"));
}
