#pragma once

#include "GameplayEffectTypes.h"
#include "PsychAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FPsychGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsCriticalHit() const {return bIsCriticalHit; }
	bool IsBlockedHit() const {return bIsBlockedHit; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsBlockedHit = bInIsCriticalHit;}
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit;}
	
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
	
	virtual FPsychGameplayEffectContext* Duplicate() const override
	{
		FPsychGameplayEffectContext* NewContext = new FPsychGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	
protected:

	UPROPERTY()
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;
};

template<>
struct TStructOpsTypeTraits<FPsychGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FPsychGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};