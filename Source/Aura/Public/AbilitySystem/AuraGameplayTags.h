// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
/**
 * 
 */
// 一个单例模式的结构体，作为属性的返回值

struct FAuraGameplayTags
{
public:
    static const FAuraGameplayTags& Get(){return AuraGameplayTags;}

    static void InitializeNativeGameplayTags();
	
	FGameplayTag Attributes_Secondary_Armor;
protected:
    
   
    
private:
    static FAuraGameplayTags AuraGameplayTags;

};