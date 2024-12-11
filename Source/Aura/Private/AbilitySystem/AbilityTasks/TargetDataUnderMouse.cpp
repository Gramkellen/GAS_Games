// Copyright kellendeng


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"


UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return MyObj;
}

// 如果是客户端控制，客户端先调用了Activate，那么发送数据给服务器
void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if(bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle AbilitySpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		// 服务器验证数据之后会调用委托进行回调
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(AbilitySpecHandle,ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(AbilitySpecHandle, ActivationPredictionKey);
		if(!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
;	
}

// 发送鼠标点击数据给服务器
void UTargetDataUnderMouse::SendMouseCursorData()
{
	// 创建预测窗口来管理能力的预测过程，在这个窗口能客户端可以进行 预测——同步的机制
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);
	// 发送TargetData给服务器,PredictionKey是一个唯一的标识
	// AbilityOriginalPredictionKey : 能力激活时的起始预测键
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	// 服务器验证数据之后会触发
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		ValidDataSignature.Broadcast(DataHandle);
	}
}

// 处理回调数据的
void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	// 服务器的TargetData应用到客户端，并清除预测的数据
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());

	// 确保能够正确广播，但是如果是先到服务器，客户端再预测，可能会广播两次同样的值？
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		ValidDataSignature.Broadcast(DataHandle);
	}
}
