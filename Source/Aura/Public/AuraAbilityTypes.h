#pragma once

#include "GameplayEffectTypes.h"

#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	virtual UScriptStruct* GetScriptStruct() const override;

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	virtual FAuraGameplayEffectContext* Duplicate() const;

	bool GetIsBlocked() const;

	void SetIsBlocked(bool bIsInBlocked);

	bool GetIsCriticalHit() const;
	
	void SetIsCriticalHit(bool bIsInCriticalHit);
	
protected:
	UPROPERTY()
	bool bIsBlocked = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

};


template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};





