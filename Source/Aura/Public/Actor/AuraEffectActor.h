// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;
class UGameplayEffect;

// 标记什么时候枚举发挥作用
UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	EAP_ApplyOnOverlap,
	EAP_ApplyOnEndOverlap,
	EAP_DoNotApply
};

// 移除 GE 的政策
UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	ERP_RemoveOnEndOverlap,
	ERP_DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor *TargetActor, TSubclassOf<UGameplayEffect>GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	bool bDestroyOnEffectRemoval = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect>InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effects")
	EEffectApplicationPolicy InstantApplicationPolicy = EEffectApplicationPolicy::EAP_DoNotApply;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect>DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effects")
	EEffectApplicationPolicy DurationApplicationPolicy = EEffectApplicationPolicy::EAP_DoNotApply;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effects")
	TSubclassOf<UGameplayEffect>InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effects")
	EEffectApplicationPolicy InfiniteApplicationPolicy = EEffectApplicationPolicy::EAP_DoNotApply;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Effects")
	EEffectRemovalPolicy InfiniteEffectRemovePolicy = EEffectRemovalPolicy::ERP_DoNotRemove;

	// 和Cueve结合可以设置不同的Level，对应Curve的X轴下标
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attribute|Combat")
	float LevelActor;
	
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveHandlesMap;
};



