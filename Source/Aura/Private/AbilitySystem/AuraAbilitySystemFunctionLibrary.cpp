// Copyright kellendeng


#include "AbilitySystem/AuraAbilitySystemFunctionLibrary.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemFunctionLibrary::GetOverlayWidgetController(const UObject* WorldContext)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContext,0);
	if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
	{
		AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
		UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
		UAttributeSet* AttributeSet = PlayerState->GetAttribute();
		FWidgetControllerParams Params(PlayerController,PlayerState,AbilitySystemComponent,AttributeSet);
		return AuraHUD->GetOverlayWidgetController(Params);
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemFunctionLibrary::GetAttributeMenuWidgetController(const UObject* WorldContext)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContext,0);
	if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
	{
		AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
		UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
		UAttributeSet* AttributeSet = PlayerState->GetAttribute();
		FWidgetControllerParams Params(PlayerController,PlayerState,AbilitySystemComponent,AttributeSet);
		return AuraHUD->GetAttributeMenuWidgetController(Params);
	}
	return nullptr;
}
