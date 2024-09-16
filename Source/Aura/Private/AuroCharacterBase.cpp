// Copyright kellendeng


#include "AuroCharacterBase.h"

// Sets default values
AAuroCharacterBase::AAuroCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAuroCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAuroCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAuroCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

