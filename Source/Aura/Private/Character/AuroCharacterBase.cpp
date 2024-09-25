// Copyright kellendeng
#include "Aura/Public/Character/AuroCharacterBase.h"


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

void AAuroCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


