// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	
	void OnHighlightActor() override;

	void UnHighlightActor() override;

	virtual void InitAbilityActorInfo() override;

	virtual  int32 GetPlayerLevel() override;
	
protected:
	void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Level")
	int32 PlayerLevel = 1;
};
