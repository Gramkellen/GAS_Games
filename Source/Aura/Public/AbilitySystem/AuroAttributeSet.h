// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuroAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class AURA_API UAuroAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAuroAttributeSet();

	// 用来定义哪些属性应当被复制，有点相当于注册，使用属性同步时需要重载
	/*
	 * GetLifetimeReplicatedProps函数，并在这个函数中使用DOREPLIFETIME或DOREPLIFETIME_CONDITION等宏来指定需要复制的属性
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_Health, Category = "Attribute|Combat")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuroAttributeSet,Health);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_MaxHealth, Category = "Attribute|Combat")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuroAttributeSet,MaxHealth);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_Mana, Category = "Attribute|Combat")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuroAttributeSet,Mana);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = Rep_MaxMana, Category = "Attribute|Combat")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuroAttributeSet,MaxMana);
	
	// 回调函数需要反射系统能够查找到
	UFUNCTION()
	void Rep_Health(const FGameplayAttributeData & OldHealth) const;

	UFUNCTION()
	void Rep_MaxHealth(const FGameplayAttributeData & OldMaxHealth) const;

	UFUNCTION()
	void Rep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void Rep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	
};