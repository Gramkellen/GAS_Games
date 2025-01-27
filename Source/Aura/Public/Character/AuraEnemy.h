// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Data/CharacterClassInfo.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UAuraUserWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	
	virtual void OnHighlightActor() override;

	virtual void UnHighlightActor() override;

	virtual void InitAbilityActorInfo() override;

	virtual  int32 GetPlayerLevel() override;

	void AttributeChangedDelegateBind();
protected:
	virtual void BeginPlay() override;

	virtual void InitializeAttributeDefaults() const override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Level")
	int32 Level = 1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Level")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent>HealthBar;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedDelegate OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedDelegate OnMaxHealthChangedDelegate;
};
