// Copyright kellendeng


#include "Player/AuroPlayerState.h"
#include "AbilitySystem/AuroAbilitySystemComponent.h"
#include "AbilitySystem/AuroAttributeSet.h"
#include "Net/UnrealNetwork.h"

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

void AAuroPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAuroPlayerState,PlayerLevel);
}

UAttributeSet* AAuroPlayerState::GetAttribute() const
{
	return Attribute;
}

void AAuroPlayerState::OnRep_PlayerLevel(int32 OldPlayerLevel)
{
	
}
