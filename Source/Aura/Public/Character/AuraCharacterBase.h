// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter,public IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuraCharacterBase();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttribute()const ;

	virtual void InitAbilityActorInfo();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<USkeletalMeshComponent>Weapon;

	UPROPERTY(VisibleAnywhere,Category ="AbilitySystem")
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite,Category="AbilitySystem")
	TObjectPtr<UAttributeSet>Attribute;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Primary Attribute")
	TSubclassOf<UGameplayEffect>DefaultPrimaryAttributesClass;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Secondary Attribute")
	TSubclassOf<UGameplayEffect>DefaultSecondaryAttributeClass;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Vital Attribute")
	TSubclassOf<UGameplayEffect>DefaultVitalAttributeClass;
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass,float Level) const;

	void InitializeDefaultAttribute() const ;

	void AddCharacterAbilities() const;
private:
	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>>StartupAbilities;
};
