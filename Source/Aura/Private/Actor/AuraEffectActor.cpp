// Copyright kellendeng


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"

AAuraEffectActor::AAuraEffectActor():LevelActor(1.f)
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root Component"));
	
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// 这里其实是 TargetASC添加了来源地GE，然后ApplyGameplayEffectToSelf
	// 获取ASC -> 创建Context描述效果的上下文消息 -> 创建效果实例 Spec -> 应用 GE
	UAbilitySystemComponent *TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	// 不直接使用Effect,通过一个Handle来管理
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, LevelActor,EffectContextHandle);
	// 这里获取返回值只是为了方便记录EndOverlap时消除对应的Infinite效果
	const FActiveGameplayEffectHandle ActiveEffectSpecHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	// Def 是一个弱指针，通过弱指针来进行访问
	bool bInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if(bInfinite && IsValid(TargetASC))
	{
		ActiveHandlesMap.Add(ActiveEffectSpecHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if(InstantApplicationPolicy == EEffectApplicationPolicy::EAP_ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if(DurationApplicationPolicy == EEffectApplicationPolicy::EAP_ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if(InfiniteApplicationPolicy == EEffectApplicationPolicy::EAP_ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if(InstantApplicationPolicy == EEffectApplicationPolicy::EAP_ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if(DurationApplicationPolicy == EEffectApplicationPolicy::EAP_ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if(InfiniteApplicationPolicy == EEffectApplicationPolicy::EAP_ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	// When EndOverlap , Remove Effect
	if(InfiniteEffectRemovePolicy == EEffectRemovalPolicy::ERP_RemoveOnEndOverlap)
	{
		UAbilitySystemComponent *TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if(TargetASC == nullptr) return;
		TArray<FActiveGameplayEffectHandle> HandlestoRemove;
		
		for(auto HandlesTuple : ActiveHandlesMap)
		{
			if (HandlesTuple.Value == TargetASC)
			{
				// 移除 效果 - 每次EndOverlap 移除一个堆叠
				TargetASC->RemoveActiveGameplayEffect(HandlesTuple.Key, 1);
				HandlestoRemove.Add(HandlesTuple.Key);
			}
		}
		// 从 Map 中进行移除
		for(FActiveGameplayEffectHandle& Handle : HandlestoRemove)
		{
			ActiveHandlesMap.FindAndRemoveChecked(Handle);
		}
	}
}



