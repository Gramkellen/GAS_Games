// Copyright kellendeng


#include "AbilitySystem/AuraGameplayTags.h"
#include "GameplayTagsManager.h"
#include "Kismet/GameplayStatics.h"

FAuraGameplayTags FAuraGameplayTags::AuraGameplayTags;

// 初始化游戏标签
void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	// 给对象添加本地标签
	/*
	 * Secondary Attributes Tags
	 */
	AuraGameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get()
												  .AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),FString("Reduce Damage"));

	AuraGameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get()
															 .AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"),FString("Break Armor"));

	AuraGameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get()
															 .AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"),FString("Block Damage Chance"));
	
	AuraGameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get()
															 .AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"),FString("The Chance of Add Damage For Enemy"));
	
	AuraGameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get()
															 .AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"),FString("Add Damage For Enemy"));
	
	AuraGameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get()
															 .AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"),FString("Reduce Critical Hit Damage"));

	AuraGameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get()
															 .AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"),FString("Add Health"));

	AuraGameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get()
															 .AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"),FString("Add Mana"));

	AuraGameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get()
														 .AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"),FString("Max Health"));

	AuraGameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get()
														 .AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"),FString("Max Mana"));

	/*
	 *  Vital Attributes Tags
	 */
	AuraGameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"),FString("Health"));

	AuraGameplayTags.Attributes_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Mana"),FString("Mana"));

	
	/*
	 * Primary Attributes Tags
	 */

	AuraGameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"),FString("Strength(Add Damage)"));

	AuraGameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"),FString("Vigor"));

	AuraGameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"),FString("Intelligence"));

	AuraGameplayTags.Attributes_Primary_Resilience  = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),FString("Resilience"));

	/*
	 * Input Tags
	 */
	AuraGameplayTags.Input_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"),FString("Ability Input key 1"));
	AuraGameplayTags.Input_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"),FString("Ability Input key 2"));
	AuraGameplayTags.Input_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"),FString("Ability Input key 3"));
	AuraGameplayTags.Input_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"),FString("Ability Input key 4"));
	AuraGameplayTags.Input_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"),FString("Ability Input Left Mouse Button"));
	AuraGameplayTags.Input_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"),FString("Ability Input Right Mouse Button"));

	// Damage Tag Add
	AuraGameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage Tag"));
	AuraGameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"),FString("Damage_Fire"));
	AuraGameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"),FString("雷电伤害"));
	AuraGameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"),FString("Damage_Arcane"));
	AuraGameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"),FString("物理伤害"));
	
	// Resistance Tgs Add
	AuraGameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString("Resistance to Fire Damage"));
	AuraGameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"), FString("Resistance to Lightning Damage"));
	AuraGameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"), FString("Resistance to Fire Arcane Damage"));
	AuraGameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString("Resistance to Physical Damage"));

	// Add Map for Damage and Resistance
	AuraGameplayTags.DamageTypestoResistance.Add(AuraGameplayTags.Damage_Fire, AuraGameplayTags.Attributes_Resistance_Fire);
	AuraGameplayTags.DamageTypestoResistance.Add(AuraGameplayTags.Damage_Lightning, AuraGameplayTags.Attributes_Resistance_Lightning);
	AuraGameplayTags.DamageTypestoResistance.Add(AuraGameplayTags.Damage_Arcane, AuraGameplayTags.Attributes_Resistance_Arcane);
	AuraGameplayTags.DamageTypestoResistance.Add(AuraGameplayTags.Damage_Physical, AuraGameplayTags.Attributes_Resistance_Physical);
	
	// Effect Tags
	AuraGameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("React When Damaged"));
}

