// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
/**
 * 
 */
// 一个单例模式的结构体，作为属性的返回值

struct FAuraGameplayTags
{
public:
    static const FAuraGameplayTags& Get(){return AuraGameplayTags;}

    static void InitializeNativeGameplayTags();

	/*
	 *  添加对应的属性标签
	 */
	
	/* Secondary Attributes Tags
	 */
	// 防御值
	FGameplayTag Attributes_Secondary_Armor;

	// 破甲值
	FGameplayTag Attributes_Secondary_ArmorPenetration;

	// 格挡率
	FGameplayTag Attributes_Secondary_BlockChance;

	// 暴击率
	FGameplayTag Attributes_Secondary_CriticalHitChance;

	// 暴击伤害
	FGameplayTag Attributes_Secondary_CriticalHitDamage;

	// 暴击抵抗
	FGameplayTag Attributes_Secondary_CriticalHitResistance;

	// 生命恢复
	FGameplayTag Attributes_Secondary_HealthRegeneration;

	// 法力值恢复
	FGameplayTag Attributes_Secondary_ManaRegeneration;

	// 最大生命值
	FGameplayTag Attributes_Secondary_MaxHealth;

	// 最大法力值
	FGameplayTag Attributes_Secondary_MaxMana;

	/*
	 * Vital Attributes Tags
	 */
	// 生命值
	FGameplayTag Attributes_Vital_Health;

	// 法力值
	FGameplayTag Attributes_Vital_Mana;

	/*
	 * Primary Attributes Tags
	 */
	// 力量
	FGameplayTag Attributes_Primary_Strength;

	// 恢复力
	FGameplayTag Attributes_Primary_Vigor;

	// 智力（和法力相关）
	FGameplayTag Attributes_Primary_Intelligence;

	// 抗性 (弹性）
	FGameplayTag Attributes_Primary_Resilience;
	
protected:
    
   
    
private:
    static FAuraGameplayTags AuraGameplayTags;

};