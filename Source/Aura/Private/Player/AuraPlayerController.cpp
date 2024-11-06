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

AAuraPlayerController::AAuraPlayerController():ShortPressThreshold(0.5f),AutoRunAcceptanceRadius(50.f),FollowTime(0.f),
                                               bTargeting(false),bAutoMove(false),CachedLocation(FVector::ZeroVector)
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
	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB) && !bTargeting)
	{
		APawn* ControlPawn = GetPawn();
		// 如果点一下就释放 -> 自动导航？
		if(FollowTime <= ShortPressThreshold && ControlPawn)
		{
			if(UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,ControlPawn->GetActorLocation(),CachedLocation))
			{
				for(const FVector& Point : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(Point,ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(),Point,8.f,8,FColor::Magenta);
				}
				bAutoMove = true;
			}
			
		}
		FollowTime = 0.f;
		bTargeting = false;
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
	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().Input_LMB) && !bTargeting)
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		FHitResult CursorHit;
		if(GetHitResultUnderCursor(ECC_Visibility,false,CursorHit))
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
