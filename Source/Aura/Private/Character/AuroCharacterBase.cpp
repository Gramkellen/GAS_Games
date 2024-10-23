// Copyright kellendeng
#include "Aura/Public/Character/AuroCharacterBase.h"
#include "AbilitySystemComponent.h"


// Sets default values
AAuroCharacterBase::AAuroCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandleSocket"));
}

UAbilitySystemComponent* AAuroCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuroCharacterBase::GetAttribute() const
{
	return Attribute;
}

void AAuroCharacterBase::InitAbilityActorInfo()
{
	
}

void AAuroCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuroCharacterBase::InitializePrimaryAttributes() const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultPrimaryAttributesClass);
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributesClass,1.f, ContextHandle);
	// Spec会应用自定义的效果规格，No Spec应用默认的效果
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(),GetAbilitySystemComponent());
}


