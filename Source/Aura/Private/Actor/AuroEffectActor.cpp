// Copyright kellendeng


#include "Actor/AuroEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "AbilitySystem/AuroAttributeSet.h"
#include "Components/SphereComponent.h"

AAuroEffectActor::AAuroEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AAuroEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuroEffectActor::OnOverlap);

	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAuroEffectActor::EndOverlap);
}


void AAuroEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(IAbilitySystemInterface *ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UAuroAttributeSet* AuroAttribute = Cast<UAuroAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(
			UAuroAttributeSet::StaticClass()));
		// ConstCast 和 const_cast 区别？ ConstCast是UE的宏定义，本质还是调用了 const_cast
		UAuroAttributeSet* MutableAuroAttributes = const_cast<UAuroAttributeSet*>(AuroAttribute);
		MutableAuroAttributes->SetHealth(MutableAuroAttributes->GetHealth()+25.f);
		Destroy();
		
	}
}

void AAuroEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}


