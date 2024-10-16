// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AuroCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API AAuroCharacterBase : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuroCharacterBase();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttribute()const ;

	virtual void InitAbilityActorInfo();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<USkeletalMeshComponent>Weapon;

	UPROPERTY(BlueprintReadOnly,Category="AbilitySystem")
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite,Category="AbilitySystem")
	TObjectPtr<UAttributeSet>Attribute;
};
