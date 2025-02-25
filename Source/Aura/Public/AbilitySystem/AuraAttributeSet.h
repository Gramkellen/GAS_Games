// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){};
	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
	
};

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAuraAttributeSet();

	TMap<FGameplayTag, FGameplayAttribute> AttributeInfoMap;

	// 用来定义哪些属性应当被复制，有点相当于注册，使用属性同步时需要重载
	/*
	 * GetLifetimeReplicatedProps函数，并在这个函数中使用DOREPLIFETIME或DOREPLIFETIME_CONDITION等宏来指定需要复制的属性
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Usually Clamp Value In PreAttribute Change
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// GE执行之后，获取对应的信息
	// #param FGameplayEffectModCallbackData - 需要添加 EffectExtension头文件，参考官网
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	void ShowFloatingDamageText(const FEffectProperties& Props, float Damage, bool bBlocked, bool bCriticalHit) const;
	
	// Primary Properties
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_Strength, Category = "Attributes|Primary Attribute")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Strength);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_Intelligence, Category = "Attributes|Primary Attribute")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Intelligence);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_Resilience, Category = "Attributes|Primary Attribute")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Resilience);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_Vigor, Category = "Attributes|Primary Attribute")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Vigor);

	// Second Properties
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_Armor,Category = "Attributes|Secondary Attribute")
	FGameplayAttributeData Armor;          // Reduce Damage and Improve Block Chance
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Armor);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_ArmorPenetration,Category = "Attributes|Secondary Attribute")
	FGameplayAttributeData ArmorPenetration;  // 物理穿透
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,ArmorPenetration);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_BlockChance,Category = "Attributes|Secondary Attribute")
	FGameplayAttributeData BlockChance;  // 格挡率+
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,BlockChance);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_CriticalHitChance,Category = "Attributes|Secondary Attribute")
	FGameplayAttributeData CriticalHitChance;  // 暴击率
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CriticalHitChance);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_CriticalHitDamage,Category = "Attributes|Secondary Attribute")
	FGameplayAttributeData CriticalHitDamage;  // 暴力伤害
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_CriticalHitResistance,Category = "Attributes|Secondary Attribute")
	FGameplayAttributeData CriticalHitResistance;  // 暴力抵挡
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_HealthRegeneration,Category = "Attributes|Secondary Attribute")
	FGameplayAttributeData HealthRegeneration;  // 生命值恢复
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,HealthRegeneration);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_ManaRegeneration,Category = "Attributes|Secondary Attribute")
	FGameplayAttributeData ManaRegeneration;  // 法力值恢复
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,ManaRegeneration);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_MaxHealth, Category = "Attributes|Secondary Attribute")
	FGameplayAttributeData MaxHealth;  // 最大生命值
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxHealth);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_MaxMana, Category = "Attributes|Secondary Attribute")
	FGameplayAttributeData MaxMana;  // 最大法力值
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxMana);

	// Resistance
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_FireResistance, Category="Attributes|Secondary Attribute|Resistance")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_LightningResistance, Category="Attributes|Secondary Attribute|Resistance")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_ArcaneResistance, Category="Attributes|Secondary Attribute|Resistance")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = Rep_PhysicalResistance, Category="Attributes|Secondary Attribute|Resistance")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance);

	
	// Vital Properties
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_Health, Category = "Attributes|Vital Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Health);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_Mana, Category = "Attributes|Vital Attribute")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Mana);

	// Meta Attributes
	UPROPERTY(EditDefaultsOnly, Category="Attributes|Meta Attribute")
	FGameplayAttributeData InComingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, InComingDamage);
	
	// 回调函数需要反射系统能够查找到
	UFUNCTION()
	void Rep_Health(const FGameplayAttributeData & OldHealth) const;

	UFUNCTION()
	void Rep_MaxHealth(const FGameplayAttributeData & OldMaxHealth) const;

	UFUNCTION()
	void Rep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void Rep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void Rep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void Rep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void Rep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void Rep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void Rep_Armor(const FGameplayAttributeData& OldArmor)const;
	
	UFUNCTION()
	void Rep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration)const;

	UFUNCTION()
	void Rep_BlockChance(const FGameplayAttributeData& OldBlockChance)const;

	UFUNCTION()
	void Rep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance)const;

	UFUNCTION()
	void Rep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage)const;
	
	UFUNCTION()
	void Rep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance)const;

	UFUNCTION()
	void Rep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration)const;

	UFUNCTION()
	void Rep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration)const;

	UFUNCTION()
	void Rep_FireResistance(const FGameplayAttributeData& OldResistanceFire)const;

	UFUNCTION()
	void Rep_LightningResistance(const FGameplayAttributeData& OldResistanceLightning)const;

	UFUNCTION()
	void Rep_ArcaneResistance(const FGameplayAttributeData& OldResistanceArcane)const;

	UFUNCTION()
	void Rep_PhysicalResistance(const FGameplayAttributeData& OldResistancePhysical)const;
	
private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};
