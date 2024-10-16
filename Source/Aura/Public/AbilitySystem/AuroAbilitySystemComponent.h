// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuroAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuroAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	void SetX();
public:
	void AbilityActorInfoSet();
protected:
	void EffectApplied( UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
