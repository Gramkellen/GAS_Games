// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,Category="Character Class Info")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};
