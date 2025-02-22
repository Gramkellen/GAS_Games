// Copyright kellendeng

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Input/AuraInputConfig.h"
#include "AuraPlayerController.generated.h"

class UDamageTextComponent;
class USplineComponent;
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
	
	FHitResult GetCursorHit() const;

	UFUNCTION(Client, Reliable)
	void ShowDamageText(float DamageValue, ACharacter* TargetCharacter, bool bBlocked, bool bCriticalHit);
	
protected:
	
	virtual void BeginPlay() override;

    virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;
	
	void CursorTrace();

	void AbilityInputTagPressed(FGameplayTag InputTag);

	void AbilityInputTagReleased(FGameplayTag InputTag);

	void AbilityInputTagHeld(FGameplayTag InputTag);

	bool bShiftDown;
	
	void ShiftKeyPressed();

	void ShiftKeyReleased();
	
	UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent();

	void AutoMove();
private:
	// 映射的上下文
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext>Context;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction>MoveAction;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction>ShiftAction;
	
	void Move(const FInputActionValue& ActionValue);

	// 使用TScriptInterface，具有引用计数和垃圾回收
	// TScriptInterface<IEnemyInterface> LastActor;
	IEnemyInterface* LastActor;
	
	// TScriptInterface<IEnemyInterface> CurrentActor;
	IEnemyInterface* CurrentActor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> AuraInputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	/* *
	 *  Top Down Move
	 */
	UPROPERTY(EditDefaultsOnly)
	float ShortPressThreshold;  // 持续按的阈值

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius;
	
	float FollowTime;    // 持续时间

	bool bTargeting;    // 是否点击目标（鼠标左键点击）

	bool bAutoMove;    // 自动移动

	FVector CachedLocation; // 鼠标点击的位置

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent>Spline;  // 曲线去拟合路径

	FHitResult CursorHit; // 同意保存CursorHit的事件

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
