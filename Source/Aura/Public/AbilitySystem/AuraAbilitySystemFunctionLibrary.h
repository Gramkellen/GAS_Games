// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemFunctionLibrary.generated.h"

struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
enum class ECharacterClass : uint8;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 因为是静态函数，所以需要一个参考物来进行路径获取
	UFUNCTION(BlueprintPure,Category="AbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintPure,Category="AbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeAttributeDefaults(const UObject* WorldContext, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable,Category="AbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeCommonAbilities(const UObject* WorldContext, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable,Category="AbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category="AbilitySystemLibrary|GamePlayEffect")
	static bool IsBlocked(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AbilitySystemLibrary|GamePlayEffect")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary|GamePlayEffect")
	static void SetBlocked(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool InBlocked);

	// 有意思的一点：UE的蓝图调用函数中，不添加 UPARAM(ref)，那么非const的引用就会变成 output
	UFUNCTION(BlueprintCallable, Category="AbilitySystemLibrary|GamePlayEffect")
	static void SetCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool InCriticalHit);
};
