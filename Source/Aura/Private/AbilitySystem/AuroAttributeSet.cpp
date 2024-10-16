// Copyright kellendeng


#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/AuroAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
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

void UAuroAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0,GetMaxHealth());
	}
	if(Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0,GetMaxHealth());
	}
}

void UAuroAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	// 获取Source 的 GameplayAbilitySystem
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if(IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvaterActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if(Props.SourceController == nullptr && Props.SourceAvaterActor!=nullptr)
		{
			if(const APawn* Pawn = Cast<APawn>(Props.SourceAvaterActor))
			{
				Props.SourceController = Pawn->GetController();
			}
				
		}
		if(Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvaterActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvaterActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvaterActor);
	}
}

void UAuroAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties Props;
	SetEffectProperties(Data,Props);
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
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroAttributeSet,MaxMana,OldMaxMana)
}


