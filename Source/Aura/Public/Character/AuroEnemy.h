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

protected:
	void BeginPlay() override;
};
