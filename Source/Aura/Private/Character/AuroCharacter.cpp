// Copyright kellendeng


#include "Aura/Public/Character/AuroCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

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