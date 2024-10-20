// Copyright kellendeng


#include "Aura/Public/Character/AuroCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuroAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuroPlayerController.h"
#include "Player/AuroPlayerState.h"
#include "UI/HUD/AuroHUD.h"

AAuroCharacter::AAuroCharacter()
{
	// 需要注意的是这里是GetCharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	// 将角色限制在一个平面上
	GetCharacterMovement()->bConstrainToPlane = true;
	// 起始时将角色吸附到最近的平面上，避免Start位置不在地上
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AAuroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Initialize Actor Info For Server
	InitAbilityActorInfo();
}

void AAuroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Intialize Actor Info For Client
	InitAbilityActorInfo();
}

void AAuroCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	AAuroPlayerState * AuroPlayerState = GetPlayerState<AAuroPlayerState>();
	check(AuroPlayerState);
	// 这里对应的其实是拥有者和表现者
	AuroPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuroPlayerState,this);
	AbilitySystemComponent = AuroPlayerState->GetAbilitySystemComponent();
	Attribute = AuroPlayerState->GetAttribute();
	Cast<UAuroAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	
	if(AAuroPlayerController * AuroPlayerController = Cast<AAuroPlayerController>(GetController()))
	{
		if(AAuroHUD* AuroHUD = Cast<AAuroHUD>(AuroPlayerController->GetHUD()))
		{
			AuroHUD->InitOverlay(AuroPlayerController,AuroPlayerState,AbilitySystemComponent,Attribute);
		}
	}
}
