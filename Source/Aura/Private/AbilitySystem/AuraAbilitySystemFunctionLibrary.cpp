// Copyright kellendeng


#include "AbilitySystem/AuraAbilitySystemFunctionLibrary.h"

#include "AuraAbilityTypes.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Game/AuraGameModeBase.h"
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
		const FWidgetControllerParams Params(PlayerController,PlayerState,AbilitySystemComponent,AttributeSet);
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
		const FWidgetControllerParams Params(PlayerController,PlayerState,AbilitySystemComponent,AttributeSet);
		return AuraHUD->GetAttributeMenuWidgetController(Params);
	}
	return nullptr;
}

void UAuraAbilitySystemFunctionLibrary::InitializeAttributeDefaults(const UObject* WorldContext,
	ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	UCharacterClassInfo *CharacterClassInfo = GetCharacterClassInfo(WorldContext);
	FCharacterClassDefaultInfo ClassDefaultInfo =  CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	// PrimaryAttributes初始化
 	FGameplayEffectContextHandle PrimaryAttributeContextHandle =  ASC->MakeEffectContext();
	PrimaryAttributeContextHandle.AddSourceObject(AvatarActor);  // 不添加SourceActor会导致缺少Effect来源
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	// SecondaryAttributes初始化
	FGameplayEffectContextHandle SecondaryAttributeContextHandle =  ASC->MakeEffectContext();
	SecondaryAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	// VitalAttributes初始化
	FGameplayEffectContextHandle VitalAttributeHandle = ASC->MakeEffectContext();
	VitalAttributeHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributeHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
	
}

void UAuraAbilitySystemFunctionLibrary::InitializeCommonAbilities(const UObject* WorldContext,
	UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContext);

	for(TSubclassOf<UGameplayAbility>AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(AbilityClass);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystemFunctionLibrary::GetCharacterClassInfo(const UObject* WorldContext)
{
	const AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
	if(AuraGameModeBase == nullptr) return nullptr;
	return AuraGameModeBase->CharacterClassInfo;
}

bool UAuraAbilitySystemFunctionLibrary::IsBlocked(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetIsBlocked();
	}
	return false;
}

bool UAuraAbilitySystemFunctionLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetIsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemFunctionLibrary::SetBlocked(FGameplayEffectContextHandle& EffectContextHandle, bool InBlocked)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsBlocked(InBlocked);
	}
}

void UAuraAbilitySystemFunctionLibrary::SetCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool InCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsCriticalHit(InCriticalHit);
	}
}
