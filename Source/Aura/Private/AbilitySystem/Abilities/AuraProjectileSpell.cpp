// Copyright kellendeng


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	// 确保只有服务器能够生成
	// 技能的真正执行（如数据变化、状态应用等）是由服务器来处理的, 客户端触发的话服务器会进行执行和验证
	if(!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	
	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		FTransform SpawnTransform;
		const FVector StartLocation(CombatInterface->GetWeaponSocketLocation());
		SpawnTransform.SetLocation(StartLocation);
		// 这里就是获取Direction的数据然后设置旋转
		FRotator TargetRotation((ProjectileTargetLocation - StartLocation).Rotation());
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
		FGameplayEffectContextHandle ContextHandle =  SourceASC->MakeEffectContext();
		ContextHandle.AddSourceObject(Projectile);
		const FGameplayEffectSpecHandle DamageSpecHandle =  SourceASC->MakeOutgoingSpec(DamageEffectClass,Level,ContextHandle);

		// 获取所有的Tags
		// 动态设置SetByCaller的值，可以通过Spec访问
		for(auto& Pair:DamageTypes)
		{
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,Pair.Key, Pair.Value.GetValueAtLevel(Level));
		}

		Projectile->DamageEffectSpecHandle = DamageSpecHandle;

		//add Debuff
		const FGameplayEffectSpecHandle DeBuffSpecHandle = SourceASC->MakeOutgoingSpec(DeBuffEffectClass,Level,ContextHandle);
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
