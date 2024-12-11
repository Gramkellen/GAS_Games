// Copyright kellendeng


#include "Player/AuraPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraGameplayTags.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "GameFramework/Pawn.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController():
		bShiftDown(false),
		LastActor(nullptr),
		CurrentActor(nullptr),
		ShortPressThreshold(0.5f),
		AutoRunAcceptanceRadius(50.f),
		FollowTime(0.f),
		bTargeting(false),
		bAutoMove(false),
		CachedLocation(FVector::ZeroVector)
{
	bReplicates = true;
	bEnableClickEvents = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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
	AutoMove();
}

void AAuraPlayerController::AutoMove()
{
	if(!bAutoMove) return;
	if(APawn* ControlledPawn = GetPawn())
	{
		// 找到样条上距离玩家最近的点
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(),ESplineCoordinateSpace::World);
		// 找到这个点的切向方向
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline,ESplineCoordinateSpace::World);

		ControlledPawn->AddMovementInput(Direction);

		const float Radius = (LocationOnSpline - CachedLocation).Length();
		if(Radius <= AutoRunAcceptanceRadius)
		{
			bAutoMove = false;
		}
	}
}

FHitResult AAuraPlayerController::GetCursorHit() const
{
	return CursorHit;
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,CursorHit);
	if(!CursorHit.bBlockingHit) return ; // No Collision
	LastActor = CurrentActor;
	CurrentActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	// 如果相等，都是 Highlighted / UnHighlighted，可以不需要进行操作
	if(LastActor!=CurrentActor)
	{
		if(LastActor) LastActor->UnHighlightActor();
		if(CurrentActor)CurrentActor->OnHighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB))
	{
		bTargeting = CurrentActor ? true : false;
		bAutoMove = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(!bTargeting && !bShiftDown && InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB) )
	{
		APawn* ControlPawn = GetPawn();
		// 如果点一下就释放 -> 自动导航？
		if(FollowTime <= ShortPressThreshold && ControlPawn)
		{
			if(UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,ControlPawn->GetActorLocation(),CachedLocation))
			{
				// 一定要记得清空，上一次生成的样条线可能残留，影响最终效果
				Spline->ClearSplinePoints();
				for(const FVector& Point : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(Point,ESplineCoordinateSpace::World);
				}
				if(!NavPath->PathPoints.IsEmpty()) // 导航路径可能存在没有生成合适的路线
				{
					CachedLocation = NavPath->PathPoints[NavPath->PathPoints.Num() -1 ]; // 避免点击点无法到达的时候，生成的路线和目标点有距离，形成死循环
					bAutoMove = true;
				}
			}
		}
		FollowTime = 0.f;
	}
	else
	{
		if(GetAuraAbilitySystemComponent() == nullptr) return;
		GetAuraAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
	}
	
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// 此时就是移动了
	if(!bTargeting && !bShiftDown && InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB))
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		if(CursorHit.bBlockingHit)
		{
			CachedLocation = CursorHit.ImpactPoint;
		}
		
		if(APawn* ControlPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedLocation - ControlPawn->GetActorLocation()).GetSafeNormal();
			ControlPawn->AddMovementInput(WorldDirection);
		}
	}
	else
	{
		if(GetAuraAbilitySystemComponent() == nullptr) return;
		GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
	}
	
}

void AAuraPlayerController::ShiftKeyPressed()
{
	bShiftDown = true;
}

void AAuraPlayerController::ShiftKeyReleased()
{
	bShiftDown = false;
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraAbilitySystemComponent()
{
	if(AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UAuraInputComponent *AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started,this, &AAuraPlayerController::ShiftKeyPressed);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Completed,this, &AAuraPlayerController::ShiftKeyReleased);
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
