// Copyright kellendeng
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraGameplayTags.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(100.f);
	InitMana(60.f);
	/**
	 * Add Primary Map
	 */
	AttributeInfoMap.Add(FAuraGameplayTags::Get().Attributes_Primary_Strength,GetStrengthAttribute());
	AttributeInfoMap.Add(FAuraGameplayTags::Get().Attributes_Primary_Vigor,GetVigorAttribute());
	AttributeInfoMap.Add(FAuraGameplayTags::Get().Attributes_Primary_Intelligence,GetIntelligenceAttribute());
	AttributeInfoMap.Add(FAuraGameplayTags::Get().Attributes_Primary_Resilience,GetResilienceAttribute());
	
	/**
	* Add Primary Map
	*/
	AttributeInfoMap.Add(FAuraGameplayTags::Get().Attributes_Secondary_Armor,GetArmorAttribute());
	AttributeInfoMap.Add(FAuraGameplayTags::Get().Attributes_Secondary_ArmorPenetration,GetArmorPenetrationAttribute());
	AttributeInfoMap.Add(FAuraGameplayTags::Get().Attributes_Secondary_BlockChance,GetBlockChanceAttribute());
	AttributeInfoMap.Add(FAuraGameplayTags::Get().Attributes_Secondary_CriticalHitChance,GetCriticalHitChanceAttribute());
	AttributeInfoMap.Add(FAuraGameplayTags::Get().Attributes_Secondary_CriticalHitDamage,GetCriticalHitDamageAttribute());
	AttributeInfoMap.Add(FAuraGameplayTags::Get().Attributes_Secondary_CriticalHitResistance,GetCriticalHitResistanceAttribute());
	AttributeInfoMap.Add(FAuraGameplayTags::Get().Attributes_Secondary_HealthRegeneration,GetHealthRegenerationAttribute());
	AttributeInfoMap.Add(FAuraGameplayTags::Get().Attributes_Secondary_ManaRegeneration,GetManaRegenerationAttribute());
	AttributeInfoMap.Add(FAuraGameplayTags::Get().Attributes_Secondary_MaxHealth,GetMaxHealthAttribute());
	AttributeInfoMap.Add(FAuraGameplayTags::Get().Attributes_Secondary_MaxMana,GetMaxManaAttribute());
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 默认是 Changed才通知，Always是希望更新值，但是值和原来一样的时候也能做些事情
	// 这里是将网络同步的属性进行注册
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Health,COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Strength,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Intelligence,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Resilience,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Vigor,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Armor,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,ArmorPenetration,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,BlockChance,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,CriticalHitChance,COND_None,REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,CriticalHitDamage,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,CriticalHitResistance,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,HealthRegeneration,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,ManaRegeneration,COND_None,REPNOTIFY_Always);
}


// CurrentValue被NewValue改变之前进行调用
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0,GetMaxHealth());
	}
	if(Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0,GetMaxMana());
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
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
void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties Props;
	SetEffectProperties(Data,Props);
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// GEngine->AddOnScreenDebugMessage(1,2.0f,FColor::Red,FString::Printf(TEXT("NewValue : %f"),GetHealth()));
		UE_LOG(LogTemp,Warning,TEXT("The Health of %s is %f"),*Props.SourceAvaterActor->GetName(),GetHealth());
		SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));
	}
	if(Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		UE_LOG(LogTemp,Warning,TEXT("The Mana of %s is %f"),*Props.SourceAvaterActor->GetName(),GetMana());
		SetMana(FMath::Clamp(GetMana(),0.f,GetMaxMana()));
	}
	
	if(Data.EvaluatedData.Attribute == GetInComingDamageAttribute())
	{
		const float LocalValue = GetInComingDamage();
		SetInComingDamage(0.f);
		if(LocalValue > 0.f)
		{
			const float NewHealth = GetHealth() - LocalValue;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bDied = NewHealth <= 0.f;

			if(!bDied)
			{
				// 放在属性这里方便进行扩展，而且可能有非碰撞造成的伤害
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
				GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);
			}
		}
	}
}

// GAMEPLAYATTRIBUTE_REPNOTIFY 宏用来通知客户端服务器的属性已经改变
// 网络复制属性时回调以下对应的函数
void UAuraAttributeSet::Rep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Health,OldHealth);
}
void UAuraAttributeSet::Rep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxHealth,OldMaxHealth);
}

void UAuraAttributeSet::Rep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldMana);
}

void UAuraAttributeSet::Rep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxMana,OldMaxMana)
}

void UAuraAttributeSet::Rep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Strength,OldStrength)
}

void UAuraAttributeSet::Rep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Intelligence,OldIntelligence);
}

void UAuraAttributeSet::Rep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Resilience,OldResilience);
}

void UAuraAttributeSet::Rep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Vigor,OldVigor);
}

void UAuraAttributeSet::Rep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Armor,OldArmor);
}

void UAuraAttributeSet::Rep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,ArmorPenetration,OldArmorPenetration);
}

void UAuraAttributeSet::Rep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,BlockChance,OldBlockChance);
}

void UAuraAttributeSet::Rep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,CriticalHitChance,OldCriticalHitChance);
}

void UAuraAttributeSet::Rep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,CriticalHitDamage,OldCriticalHitDamage);
}

void UAuraAttributeSet::Rep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,CriticalHitResistance,OldCriticalHitResistance);
}

void UAuraAttributeSet::Rep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,HealthRegeneration,OldHealthRegeneration);
}

void UAuraAttributeSet::Rep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,ManaRegeneration,OldManaRegeneration);
}


