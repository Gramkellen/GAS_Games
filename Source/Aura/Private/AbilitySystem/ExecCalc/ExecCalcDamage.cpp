// Copyright kellendeng


#include "AbilitySystem/ExecCalc/ExecCalcDamage.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraGameplayTags.h"
#include "Character/AuraCharacterBase.h"
#include "Character/AuraEnemy.h"

struct AuraDamageStatics
{
	// 辅助宏：声明捕获的变量
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);

	AuraDamageStatics()
	{
		// 初始化时定义捕获的规则, 这里是获取了 Effect目标的 Armor，比如可以减少对方的护甲这样
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalcDamage::UExecCalcDamage()
{
	// UGameplayEffectCalculation的数组，去捕获对应的属性
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
}

void UExecCalcDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                             FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec EffectSpec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	// 获取Target的 Block属性
	float BlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParameters,BlockChance);
	float RandomValue = FMath::RandRange(0,100);
	bool isBlocked = BlockChance > RandomValue ? true : false;
	UE_LOG(LogTemp,Warning, TEXT("RandomValue = %f ,BlockChange = %f"),RandomValue, BlockChance);
	
	float Damage = EffectSpec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().DamageTag);
	Damage = isBlocked ? Damage * 0.5 : Damage;

	// 添加 Armor && ArmorPenetration
	float TargetArmor = 0.f, SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParameters, TargetArmor);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluateParameters, SourceArmorPenetration);
	UE_LOG(LogTemp, Warning, TEXT("SourceArmorPenetration = %f"),SourceArmorPenetration);
	SourceArmorPenetration = FMath::Min<float>(SourceArmorPenetration,100.f);
	float EffectArmor = TargetArmor * (100.f - SourceArmorPenetration)/ 100.f;
	UE_LOG(LogTemp, Warning, TEXT("EffectArmor = %f"),EffectArmor);
	Damage = Damage * (100 - EffectArmor) / 100.f;
	// 目标属性来叠加这个值
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetInComingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
