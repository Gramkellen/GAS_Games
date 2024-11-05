// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Input/AuraInputConfig.h"
#include "AuraPlayerController.generated.h"

class UAuraAbilitySystemComponent;
class IEnemyInterface;
class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

protected:
	virtual void BeginPlay() override;

    virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;
	
	void CursorTrace();

	void AbilityInputTagPressed(FGameplayTag InputTag);

	void AbilityInputTagReleased(FGameplayTag InputTag);

	void AbilityInputTagHeld(FGameplayTag InputTag);

	UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent();
private:
	// 映射的上下文
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext>Context;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction>MoveAction;

	void Move(const FInputActionValue& ActionValue);

	// 使用TScriptInterface，具有引用计数和垃圾回收
	TScriptInterface<IEnemyInterface>LastActor;
	
	TScriptInterface<IEnemyInterface>CurrentActor;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> AuraInputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
};
