// Copyright kellendeng


#include "Aura/Public/Character/AuroEnemy.h"

#include "Aura/Aura.h"

AAuroEnemy::AAuroEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECR_Block);
	
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
