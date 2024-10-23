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

	UAttributeSet* GetAttribute()const ;
protected:
	UPROPERTY(VisibleAnywhere,Category="AbilitySystem")
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite,Category="AbilitySystem")
	TObjectPtr<UAttributeSet>Attribute;
};
