// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuroEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class AURA_API AAuroEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuroEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor *Target, TSubclassOf<UGameplayEffect>GameplayEffectClass);
	
	UPROPERTY(EditAnywhere,Category = "Effects")
	TSubclassOf<UGameplayEffect>InstanceGameplayEffectClass;
};



