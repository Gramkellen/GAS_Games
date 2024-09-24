// Copyright kellendeng


#include "Player/AuroPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AAuroPlayerController::AAuroPlayerController()
{
	bReplicates = true;
}

void AAuroPlayerController::BeginPlay()
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

void AAuroPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuroPlayerController::Move);
}

// 以前就是Bind MoveForward Bind MoveRight这样
void AAuroPlayerController::Move(const FInputActionValue& ActionValue)
{
	// FInputAction起始是一个对应上和右的一个结构体
	const FVector2D InputAxisVector = ActionValue.Get<FVector2D>();
	FRotator Rotation = GetControlRotation();
	FRotator YawRotation{0.f,Rotation.Yaw,0.f};

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if(APawn* PawnController = GetPawn<APawn>())
	{
		// 对应了WSAD
		PawnController->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		PawnController->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}
