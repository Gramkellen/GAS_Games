// Copyright kellendeng


#include "AbilitySystem/ExecCalc/ExecCalcDamage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/AuraAbilitySystemFunctionLibrary.h"
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
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	
	AuraDamageStatics()
	{
		// 初始化时定义捕获的规则, 这里是获取了 Effect目标的 Armor，比如可以减少对方的护甲这样
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		
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
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalcDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                             FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);
	
	// 初始化参数
	const FGameplayEffectSpec EffectSpec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	FGameplayEffectContextHandle ContextHandle = EffectSpec.GetEffectContext();
	
	// 获取Target的 Block属性，并根据 Block修改Damage
	float BlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParameters,BlockChance);
	float RandomValue = FMath::RandRange(0,100);
	bool isBlocked = BlockChance > RandomValue ? true : false;
	UAuraAbilitySystemFunctionLibrary::SetBlocked(ContextHandle, isBlocked);
	UE_LOG(LogTemp,Warning, TEXT("RandomValue = %f ,BlockChange = %f"),RandomValue, BlockChance);
	float Damage = 0.f;
	// 获取 Caller 中存在的所有的伤害类型
	for(FGameplayTag DamageTag:FAuraGameplayTags::Get().DamageTypes)
	{
		Damage += EffectSpec.GetSetByCallerMagnitude(DamageTag);
	}
	Damage = isBlocked ? Damage * 0.5 : Damage;

	// 添加 Armor && ArmorPenetration
	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemFunctionLibrary::GetCharacterClassInfo(SourceAvatar);
	FRealCurve* ArmorPenetrationCoefficientCurve =  CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	FRealCurve* EffectArmorCoefficientCurve =  CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectArmor"), FString());
	float TargetArmor = 0.f;
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParameters, TargetArmor);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluateParameters, SourceArmorPenetration);
	
	UE_LOG(LogTemp, Warning, TEXT("SourceArmorPenetration = %f"),SourceArmorPenetration);
	SourceArmorPenetration = FMath::Min<float>(SourceArmorPenetration,100.f);
	const float ArmorPenetrationCoefficient = ArmorPenetrationCoefficientCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	float EffectArmor = TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient)/ 100.f;
	
	UE_LOG(LogTemp, Warning, TEXT("EffectArmor = %f"),EffectArmor);
	const float EffectArmorCoefficient = EffectArmorCoefficientCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	Damage = Damage * (100 - EffectArmor * EffectArmorCoefficient) / 100.f;

	// Critical Hit 判定
	float SourceCriticalHitChance = 0.f;
	float SourceCriticalHitDamage = 0.f;
	float TargetCriticalHitResistance = 0.f;
	const FRealCurve* CriticalHitResistanceCoefficientCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"),FString());
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParameters, SourceCriticalHitChance);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParameters, SourceCriticalHitDamage);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParameters, TargetCriticalHitResistance);
	float EffectCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficientCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	bool isCritical = EffectCriticalHitChance > FMath::RandRange(1 ,100);
	UAuraAbilitySystemFunctionLibrary::SetCriticalHit(ContextHandle, isCritical);
	Damage = isCritical? Damage * 2 + SourceCriticalHitDamage : Damage;
	
	// 目标属性来叠加这个值
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetInComingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
