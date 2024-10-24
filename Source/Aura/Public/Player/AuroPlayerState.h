// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AuroPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
UCLASS()
class AURA_API AAuroPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuroPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 网络同步需要调用
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UAttributeSet* GetAttribute()const ;

	FORCEINLINE int32 GetPlayerLevel() const {return PlayerLevel;};
protected:
	UPROPERTY(VisibleAnywhere,Category="AbilitySystem")
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite,Category="AbilitySystem")
	TObjectPtr<UAttributeSet>Attribute;

private:
	UPROPERTY(VisibleAnywhere,ReplicatedUsing = OnRep_PlayerLevel)
	int32 PlayerLevel = 1;

	UFUNCTION()
	void OnRep_PlayerLevel(int32 OldPlayerLevel);
};
