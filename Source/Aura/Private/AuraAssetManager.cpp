// Copyright kellendeng


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FAuraGameplayTags::InitializeNativeGameplayTags();
	// 要使用TargetData，先进行初始化
	UAbilitySystemGlobals::Get().InitGlobalData();
}
