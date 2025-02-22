// Copyright kellendeng


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Projects.h"
#include "AbilitySystem/AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	// 确保只有服务器能够生成
	if(!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	
	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		FTransform SpawnTransform;
		const FVector StartLocation(CombatInterface->GetWeaponSocketLocation());
		SpawnTransform.SetLocation(StartLocation);
		// 这里就是获取Direction的数据然后设置旋转
		FRotator TargetRotation((ProjectileTargetLocation - StartLocation).GetSafeNormal().Rotation());
		TargetRotation.Pitch = 0.f;
		SpawnTransform.SetRotation(TargetRotation.Quaternion());
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// Apply DamageEffect and DeBuff
		const float Level = GetAbilityLevel();
		UAbilitySystemComponent* SourceASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectContextHandle ContextHandle =  SourceASC->MakeEffectContext();
		const FGameplayEffectSpecHandle DamageSpecHandle =  SourceASC->MakeOutgoingSpec(DamageEffectClass,Level,ContextHandle);

		// 获取所有的Tags
		// 动态设置SetByCaller的值，可以通过Spec访问
		for(auto& Pair:DamageTypes)
		{
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,Pair.Key, Pair.Value.GetValueAtLevel(Level));
		}
		
		const FGameplayEffectSpecHandle DeBuffSpecHandle = SourceASC->MakeOutgoingSpec(DeBuffEffectClass,Level,ContextHandle);
		Projectile->DamageEffectSpecHandle = DamageSpecHandle;
		if(DeBuffSpecHandle.IsValid() && DeBuffSpecHandle.Data.IsValid())
		{
			SourceASC->ApplyGameplayEffectSpecToSelf(*DeBuffSpecHandle.Data.Get());  // 释放Ability就减少Mana
		}
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
