// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuroAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FAssetTagsDelegate,const FGameplayTagContainer&);
/**
 * 
 */
UCLASS()
class AURA_API UAuroAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityActorInfoSet();

	FAssetTagsDelegate AssetTagsDelegate;
protected:
	void EffectApplied( UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
