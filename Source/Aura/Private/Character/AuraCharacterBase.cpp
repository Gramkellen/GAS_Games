// Copyright kellendeng
#include "Aura/Public/Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandleSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);   // 设置 Overlap的事件
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraCharacterBase::GetAttribute() const
{
	return AttributeSet;
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
	
}

UAnimMontage* AAuraCharacterBase::GetHieReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Died()
{
	// 如果采用DetachFromActor,分离后不受Actor控制了，影响比较全，非局部影响
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MultiCastDied();
}

void AAuraCharacterBase::MultiCastDied_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic,ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // 否则会和 mesh冲突 / 角色离地面有距离
	Dissolve();
}

void AAuraCharacterBase::Dissolve()
{
	if(IsValid(DissolveMaterialInstance))
	{
		// 转成Dynamic的Material主要是为了在运行时修改参数，静态的材料是没办法修改的
		UMaterialInstanceDynamic *DissolveDynamicInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DissolveDynamicInstance);
		StartDissolveTimeline(DissolveDynamicInstance);
	}

	if(IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic *WeaponDissolveDynamicInstance = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, WeaponDissolveDynamicInstance);
		StartWeaponDissolveTimeline(WeaponDissolveDynamicInstance);
	}
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector AAuraCharacterBase::GetWeaponSocketLocation()
{
	check(Weapon);
	check(WeaponTipSocketName.IsValid());
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass,Level,EffectContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitializeAttributeDefaults() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributesClass,1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributeClass,1.f);
	ApplyEffectToSelf(DefaultVitalAttributeClass,1.f);
}

void AAuraCharacterBase::AddCharacterAbilities() const
{
	if(!HasAuthority()) return;
	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraAbilitySystemComponent->AddGameplayAbilities(StartupAbilities);
}


