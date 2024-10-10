// Copyright kellendeng


#include "AbilitySystem/AuroAttributeSet.h"
#include "Net/UnrealNetwork.h"

UAuroAttributeSet::UAuroAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(30.f);
	InitMaxMana(100.f);
}

void UAuroAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 默认是 Changed才通知，ALways是希望更新值，但是值和原来一样的时候也能做些事情
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroAttributeSet,Health,COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuroAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuroAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
}

// GAMEPLAYATTRIBUTE_REPNOTIFY 宏用来通知客户端服务器的属性已经改变
void UAuroAttributeSet::Rep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroAttributeSet,Health,OldHealth);
}

void UAuroAttributeSet::Rep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroAttributeSet,MaxHealth,OldMaxHealth);
}

void UAuroAttributeSet::Rep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroAttributeSet,Mana,OldMana);
}

void UAuroAttributeSet::Rep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroAttributeSet,MaxMana,OldMaxMana);
}
