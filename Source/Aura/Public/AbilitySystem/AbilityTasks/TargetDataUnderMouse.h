// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

// const 是为了保证传播值能够复制，使用CAST时不要直接用引用
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature,const FVector&,Data); 
/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	/* HidePin = "ParameterName",隐藏指定参数的输入引脚
	 * DefaultToSelf = "ParameterName", 指定参数的值设置为 Self
	*/
	UFUNCTION(BlueprintCallable,Category="Ability|Tasks",meta = (DisplayName="TargetDataUnderMouse" , HidePin = "OwningAbility", DefaultToSelf = "OwningAbility",BlueprintInternalUseOnly="true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	/*
	 * 只能用于 Multicast ，用于在蓝图中显示并进行分配
	 */
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidDataSignature;

private:
	virtual void Activate() override;
};
