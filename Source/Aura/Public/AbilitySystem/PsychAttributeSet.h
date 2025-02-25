// Copyright Psych Ward 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "PsychAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()
	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;
	
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
	
};

/**
 * 
 */


UCLASS()
class AURA_API UPsychAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UPsychAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/*
	 * Primary Attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category= "Vital Attributes") 
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category= "Vital Attributes") 
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category= "Vital Attributes") 
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category= "Vital Attributes") 
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet, Vigor);
	
	/*
	 * Vital Attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category= "Vital Attributes") 
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet, Health);
	

	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category= "Vital Attributes") 
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet,Mana);
	


	/*
	 * Secondary Attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category= "Secondary Attributes") 
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category= "Secondary Attributes") 
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category= "Secondary Attributes") 
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category= "Secondary Attributes") 
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category= "Secondary Attributes") 
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category= "Secondary Attributes") 
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet, CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category= "Secondary Attributes") 
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category= "Secondary Attributes") 
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet, ManaRegeneration);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category= "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet,MaxMana);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category= "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPsychAttributeSet,MaxHealth);
	
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;
	
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;


	
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	
private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	
};
