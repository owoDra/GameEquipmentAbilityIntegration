// Copyright (C) 2024 owoDra

#include "EquipmentFragment_AddAbilities.h"

#include "Equipment/Equipment.h"

#include "AbilitySystemGlobals.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(EquipmentFragment_AddAbilities)

//////////////////////////////////////////////////////////////////////////////////////////

UEquipmentFragment_AddAbilities::UEquipmentFragment_AddAbilities(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EEquipmentFragmentNetExecutionPolicy::ServerOnly;

#if WITH_EDITOR
	StaticClass()->FindPropertyByName(FName{ TEXTVIEW("NetExecutionPolicy") })->SetPropertyFlags(CPF_DisableEditOnTemplate);
#endif
}

#if WITH_EDITOR 
EDataValidationResult UEquipmentFragment_AddAbilities::IsDataValid(FDataValidationContext& Context) const
{
	auto Result{ CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid) };

	Result = CombineDataValidationResults(Result, IsArrayValid(AbilitiesToAdd_OnGiven, Context, TEXT("AbilitiesToAdd_OnGiven")));
	Result = CombineDataValidationResults(Result, IsArrayValid(AbilitiesToAdd_OnEquiped, Context, TEXT("AbilitiesToAdd_OnEquiped")));

	return Result;
}

EDataValidationResult UEquipmentFragment_AddAbilities::IsArrayValid(const TArray<FAbilitiesToAdd>& InAbilitiesToAdd, FDataValidationContext& Context, FString InArrayName) const
{
	auto Result{ EDataValidationResult::Valid };

	int32 Index{ 0 };
	for (const auto& Entry : InAbilitiesToAdd)
	{
		if (Entry.bUseAbilitySet)
		{
			if (!Entry.AbilitySet)
			{
				Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

				Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid AbilitySet set in %s[%d] in %s"), *InArrayName, Index, *GetNameSafe(this))));
			}
		}
		else
		{
			for (const auto& Ability : Entry.GrantedGameplayAbilities)
			{
				if (!Ability.IsValid())
				{
					Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

					Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid Ability set in %s[%d] in %s"), *InArrayName, Index, *GetNameSafe(this))));
				}
			}

			for (const auto& Effect : Entry.GrantedGameplayEffects)
			{
				if (!Effect.IsValid())
				{
					Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

					Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid Effect set in %s[%d] in %s"), *InArrayName, Index, *GetNameSafe(this))));
				}
			}

			for (const auto& Attribute : Entry.GrantedAttributes)
			{
				if (!Attribute.IsValid())
				{
					Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

					Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid Attribute set in %s[%d] in %s"), *InArrayName, Index, *GetNameSafe(this))));
				}
			}
		}

		Index++;
	}

	return Result;
}
#endif


void UEquipmentFragment_AddAbilities::HandleEquipmentGiven()
{
	Super::HandleEquipmentGiven();

	AddAbilities(AbilitiesToAdd_OnGiven, GrantedHandles_OnGiven);
}

void UEquipmentFragment_AddAbilities::HandleEquipmentRemove()
{
	Super::HandleEquipmentRemove();

	RemoveAbilities(GrantedHandles_OnGiven);
}

void UEquipmentFragment_AddAbilities::HandleEquiped()
{
	Super::HandleEquiped();

	AddAbilities(AbilitiesToAdd_OnEquiped, GrantedHandles_OnEquiped);
}

void UEquipmentFragment_AddAbilities::HandleUnequiped()
{
	Super::HandleUnequiped();

	RemoveAbilities(GrantedHandles_OnEquiped);
}


void UEquipmentFragment_AddAbilities::AddAbilities(const TArray<FAbilitiesToAdd>& InAbilitiesToAdd, FAbilitySet_GrantedHandles& OutGrantedHandles)
{
	auto* Equipment{ GetEquipment() };

	if (auto* ASC{ UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetEquipmentOwner()) })
	{
		for (const auto& Entry : InAbilitiesToAdd)
		{
			if (Entry.bUseAbilitySet)
			{
				Entry.AbilitySet->GiveToAbilitySystem(ASC, &OutGrantedHandles, Equipment);
			}
			else
			{
				OutGrantedHandles.AddAbilities(ASC, Entry.GrantedGameplayAbilities, Equipment);
				OutGrantedHandles.AddGameplayEffects(ASC, Entry.GrantedGameplayEffects, Equipment);
				OutGrantedHandles.AddAttributeSets(ASC, Entry.GrantedAttributes, Equipment);
			}
		}
	}
}

void UEquipmentFragment_AddAbilities::RemoveAbilities(FAbilitySet_GrantedHandles& InOutGrantedHandles)
{
	if (auto* ASC{ UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetEquipmentOwner()) })
	{
		InOutGrantedHandles.TakeFromAbilitySystem(ASC);
	}
}
