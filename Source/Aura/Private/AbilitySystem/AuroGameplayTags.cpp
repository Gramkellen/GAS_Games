// Copyright kellendeng


#include "AbilitySystem/AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::AuraGameplayTags;

// 初始化游戏标签
void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	// 给对象添加本地标签
	AuraGameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),FString("Reduce Damage"));
}
