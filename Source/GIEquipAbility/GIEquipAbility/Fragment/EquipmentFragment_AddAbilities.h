// Copyright (C) 2024 owoDra

#pragma once

#include "Equipment/Fragment/EquipmentFragment.h"

#include "AbilitySet.h"

#include "EquipmentFragment_AddAbilities.generated.h"


/**
 * Entry data for adding abilities
 */
USTRUCT(BlueprintType)
struct FAbilitiesToAdd
{
	GENERATED_BODY()
public:
	FAbilitiesToAdd() {}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bUseAbilitySet{ true };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "bUseAbilitySet"))
	TObjectPtr<const UAbilitySet> AbilitySet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "!bUseAbilitySet"))
	TArray<FAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "!bUseAbilitySet"))
	TArray<FAbilitySet_GameplayEffect> GrantedGameplayEffects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "!bUseAbilitySet"))
	TArray<FAbilitySet_AttributeSet> GrantedAttributes;

};


/**
 * EquipmentFragment class to set AnimLayers for a specific mesh
 */
UCLASS(meta = (DisplayName = "Add Abilities"))
class GIEQUIPABILITY_API UEquipmentFragment_AddAbilities : public UEquipmentFragment
{
	GENERATED_BODY()
public:
	UEquipmentFragment_AddAbilities(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR 
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;

private:
	EDataValidationResult IsArrayValid(const TArray<FAbilitiesToAdd>& InAbilitiesToAdd, class FDataValidationContext& Context, FString InArrayName) const;
#endif

protected:
	//
	// List of abilities to apply when Equipment is given
	//
	UPROPERTY(EditDefaultsOnly, Category = "Add Abilities")
	TArray<FAbilitiesToAdd> AbilitiesToAdd_OnGiven;

	//
	// List of abilities to apply when Equipment is equiped
	//
	UPROPERTY(EditDefaultsOnly, Category = "Add Abilities")
	TArray<FAbilitiesToAdd> AbilitiesToAdd_OnEquiped;

protected:
	UPROPERTY(Transient)
	FAbilitySet_GrantedHandles GrantedHandles_OnGiven;

	UPROPERTY(Transient)
	FAbilitySet_GrantedHandles GrantedHandles_OnEquiped;

public:
	virtual void HandleEquipmentGiven() override;
	virtual void HandleEquipmentRemove() override;
	virtual void HandleEquiped() override;
	virtual void HandleUnequiped() override;

protected:
	void AddAbilities(const TArray<FAbilitiesToAdd>& InAbilitiesToAdd, FAbilitySet_GrantedHandles& OutGrantedHandles);
	void RemoveAbilities(FAbilitySet_GrantedHandles& InOutGrantedHandles);

};
