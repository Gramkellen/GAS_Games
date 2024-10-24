// Copyright kellendeng


#include "Aura/Public/Character/AuroEnemy.h"

#include "AbilitySystem/AuroAbilitySystemComponent.h"
#include "AbilitySystem/AuroAttributeSet.h"
#include "Aura/Aura.h"

AAuroEnemy::AAuroEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuroAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	Attribute = CreateDefaultSubobject<UAuroAttributeSet>("Attribute");
}

void AAuroEnemy::OnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	if(Weapon)
	{
		Weapon->SetRenderCustomDepth(true);
		Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
}

void AAuroEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	if(Weapon)
	{
		Weapon->SetRenderCustomDepth(false);
	}
}

void AAuroEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UAuroAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}

int32 AAuroEnemy::GetPlayerLevel()
{
	return PlayerLevel;
}

void AAuroEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}
