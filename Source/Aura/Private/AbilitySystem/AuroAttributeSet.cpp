// Copyright kellendeng
#include "AbilitySystem/AuroAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"

UAuroAttributeSet::UAuroAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitMana(60.f);
	InitMaxMana(100.f);
}

void UAuroAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 默认是 Changed才通知，ALways是希望更新值，但是值和原来一样的时候也能做些事情
	// 这里是将网络同步的属性进行注册
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroAttributeSet,Health,COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuroAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuroAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuroAttributeSet,Strength,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroAttributeSet,Intelligence,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroAttributeSet,Resilience,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroAttributeSet,Vigor,COND_None,REPNOTIFY_Always);
}


// CurrentValue被NewValue改变之前进行调用
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

// 应用GE的效果
void UAuroAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties Props;
	SetEffectProperties(Data,Props);
	if(Data.EvaluatedData.Attribute== GetHealthAttribute())
	{
		// GEngine->AddOnScreenDebugMessage(1,2.0f,FColor::Red,FString::Printf(TEXT("NewValue : %f"),GetHealth()));
		SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));
	}
	if(Data.EvaluatedData.Attribute== GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0.f,GetMaxMana()));
	}
}

// GAMEPLAYATTRIBUTE_REPNOTIFY 宏用来通知客户端服务器的属性已经改变
// 网络复制属性时回调以下对应的函数
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

void UAuroAttributeSet::Rep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroAttributeSet,Strength,OldStrength)
}

void UAuroAttributeSet::Rep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroAttributeSet,Intelligence,OldIntelligence);
}

void UAuroAttributeSet::Rep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroAttributeSet,Resilience,OldResilience);
}

void UAuroAttributeSet::Rep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroAttributeSet,Vigor,OldVigor);
}


