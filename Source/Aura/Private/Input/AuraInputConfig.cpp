// Copyright kellendeng


#include "Input/AuraInputConfig.h"


const UInputAction* UAuraInputConfig::FindAbilityInputActionByTag(const FGameplayTag& GameplayTag, bool bLogNotFound) const
{
	for(const FAuraInputAction& AuraInputAction:GameplayAbilityInputActions)
	{
		if(AuraInputAction.InputAction && AuraInputAction.InputTag.MatchesTagExact(GameplayTag))
		{
			return AuraInputAction.InputAction;
		}
	}
	if(bLogNotFound)
	{
		UE_LOG(LogTemp,Error,TEXT("Can not FInd This Action By Tag"));
	}

	return nullptr;
}
