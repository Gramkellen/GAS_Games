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

void AAuroCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	const FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass,1.f,EffectContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void AAuroCharacterBase::InitializeDefaultAttribute() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributesClass,1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributeClass,1.f);
}


