// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FAssetTagsDelegate,const FGameplayTagContainer&);
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityActorInfoSet();

	FAssetTagsDelegate AssetTagsDelegate;

	void AddGameplayAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities) ;

	void AbilityInputTagPressed(const FGameplayTag& InputTag);

	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	
protected:
	UFUNCTION(Client,Reliable)
	void ClientEffectApplied( UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) ;
};
