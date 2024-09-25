// Copyright kellendeng


#include "Player/AuroPlayerState.h"

#include "AbilitySystem/AuroAbilitySystemComponent.h"
#include "AbilitySystem/AuroAttributeSet.h"

AAuroPlayerState::AAuroPlayerState()
{
	NetUpdateFrequency = 100.f;
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuroAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	Attribute = CreateDefaultSubobject<UAuroAttributeSet>("Attribute");
}

UAbilitySystemComponent* AAuroPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuroPlayerState::GetAttribute() const
{
	return Attribute;
}
