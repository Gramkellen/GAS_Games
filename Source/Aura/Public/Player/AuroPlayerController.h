// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "AuroPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class AURA_API AAuroPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuroPlayerController();

protected:
	void BeginPlay() override;

    void SetupInputComponent() override;
private:
	// 映射的上下文
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext>Context;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction>MoveAction;

	void Move(const FInputActionValue& ActionValue);
	
};
