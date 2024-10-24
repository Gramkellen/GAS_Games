// Copyright kellendeng


#include "AbilitySystem/MMC/MMC_MaxHealth.h"

#include "AbilitySystem/AuroAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuroAttributeSet::GetVigorAttribute();
	// 可以选择捕获施法者还是目标的属性
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef); // 继承而来的，一个Definition的数组
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 属性聚合器，来控制属性的计算方式 -> 比如属性还有Tag这些
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef,Spec,EvaluateParameters,Vigor);
	Vigor = FMath::Max<float>(Vigor,0.f);

	// 可能会没有设置 SourceObject
	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject()))
	{
		const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
		return 80.f + Vigor * 1.5f + PlayerLevel * 10.f;
	}
 
	return Vigor;
	
}
