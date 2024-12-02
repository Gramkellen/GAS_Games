// Copyright kellendeng


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return MyObj;
}

// 感觉应该在Activate的时候绑定，而不是这个时候广播？ 目前有点小Bug
void UTargetDataUnderMouse::Activate()
{
	// FVector MouseLocation;
	AAuraPlayerController *PlayerController = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	if(PlayerController->GetCursorHit().bBlockingHit)
	{
		const FVector MouseLocation(PlayerController->GetCursorHit().Location);
		ValidDataSignature.Broadcast(MouseLocation);
	}
	
}
