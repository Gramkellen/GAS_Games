// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "Character/AuroCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuroEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuroEnemy : public AAuroCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuroEnemy();
	
	void OnHighlightActor() override;

	void UnHighlightActor() override;

	virtual void InitAbilityActorInfo() override;

	virtual  int32 GetPlayerLevel() override;
	
protected:
	void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Level")
	int32 PlayerLevel = 1;
};
