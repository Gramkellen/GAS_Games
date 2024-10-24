// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "Character/AuroCharacterBase.h"
#include "AuroCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuroCharacter : public AAuroCharacterBase
{
	GENERATED_BODY()
public:
	AAuroCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	virtual void InitAbilityActorInfo() override;

	virtual int32 GetPlayerLevel() override;
};
