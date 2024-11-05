// Copyright kellendeng


#include "Player/AuraPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	bEnableClickEvents = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(Context);
	// 获取本地玩家的一个输入系统
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem!=nullptr)
	{
		Subsystem->AddMappingContext(Context,0);
		bShowMouseCursor = true;
		DefaultMouseCursor = EMouseCursor::Default;

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
	}
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,CursorHit);
	if(!CursorHit.bBlockingHit) return ; // No Collision
	LastActor = CurrentActor;
	CurrentActor = CursorHit.GetActor();
	/* Diffrent Case About Hit Result
	 * A. both nullptr
	 * B. last nullptr but current not nullptr
	 * C. last not nullptr but current is nullptr
	 * D. last not nullptr and current not nullptr
	 *	- last == current
	 *	- last != current
	 */
	if(LastActor == nullptr)
	{
		if(CurrentActor!=nullptr)
		{
			CurrentActor->OnHighlightActor();
		}
	}
	else
	{
		if(CurrentActor!=nullptr)
		{
			if(CurrentActor!=LastActor)
			{
				LastActor->UnHighlightActor();
				CurrentActor->OnHighlightActor();
			}
		}
		else
		{
			LastActor->UnHighlightActor();
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag GameplayTag)
{
	GEngine->AddOnScreenDebugMessage(1,3.f,FColor::Red,*GameplayTag.ToString());
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag GameplayTag)
{
	GEngine->AddOnScreenDebugMessage(2,3.f,FColor::Blue,*GameplayTag.ToString());
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag GameplayTag)
{
	GEngine->AddOnScreenDebugMessage(3,3.f,FColor::Green,*GameplayTag.ToString());
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UAuraInputComponent *AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);

	AuraInputComponent->BindAbilityInputActions(AuraInputConfig,this,&ThisClass::AbilityInputTagPressed,&ThisClass::AbilityInputTagReleased,&ThisClass::AbilityInputTagHeld);
}

// 以前就是Bind MoveForward Bind MoveRight这样
void AAuraPlayerController::Move(const FInputActionValue& ActionValue)
{
	// FInputAction起始是一个对应上和右的一个结构体
	const FVector2D InputAxisVector = ActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation{0.f,Rotation.Yaw,0.f};

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if(APawn* PawnController = GetPawn<APawn>())
	{
		// 对应了WSAD
		PawnController->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		PawnController->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}
