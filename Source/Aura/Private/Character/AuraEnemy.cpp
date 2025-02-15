// Copyright kellendeng


#include "Aura/Public/Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemFunctionLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraGameplayTags.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy():LifeSpan(5.f)   //HealthBarWidth(135.f),HealthBarHeight(13.f)
{
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("Attribute");
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	// HealthBar->SetDrawSize(FVector2D(HealthBarWidth,HealthBarHeight));
	HealthBar->SetDrawAtDesiredSize(true);
	HealthBar->SetRelativeLocation(FVector(0.f,0.f,70.f));
}

void AAuraEnemy::OnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	if(Weapon)
	{
		Weapon->SetRenderCustomDepth(true);
		Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	if(Weapon)
	{
		Weapon->SetRenderCustomDepth(false);
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::AttributeChangedDelegateBind()
{
	// 绑定 HealthBar的 WidgetController
	if(UAuraUserWidget* AuraUserWidget = CastChecked<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	// 绑定属性变化的委托事件
	if(const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChangedDelegate.Broadcast(Data.NewValue);
			});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
			});

		// 与指定GameplayTag相关事件发生时进行回调
		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact,
														EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AAuraEnemy::HitReactTagChanged
		);
		// 广播初始值
		OnHealthChangedDelegate.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChangedDelegate.Broadcast(AuraAttributeSet->GetMaxHealth());
	}

	
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallBackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0 ? true : false;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AAuraEnemy::Died()
{
	SetLifeSpan(LifeSpan);
	Super::Died();
}

void AAuraEnemy::BeginPlay()
{
	// 设置初始的最大速度
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed; 
	Super::BeginPlay();
	InitAbilityActorInfo();
	InitializeAttributeDefaults(); 
	AttributeChangedDelegateBind();
	UAuraAbilitySystemFunctionLibrary::InitializeCommonAbilities(this,AbilitySystemComponent);
}

void AAuraEnemy::InitializeAttributeDefaults() const
{
	UAuraAbilitySystemFunctionLibrary::InitializeAttributeDefaults(this, CharacterClass, Level, AbilitySystemComponent);
}
