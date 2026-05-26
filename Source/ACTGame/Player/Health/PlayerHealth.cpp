#include "PlayerHealth.h"

#include "Math/UnrealMathUtility.h"

UPlayerHealth::UPlayerHealth()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerHealth::BeginPlay()
{
	Super::BeginPlay();

	MaxHealth = FMath::Max(MaxHealth, 0.1f);
	CurrentHealth = MaxHealth;
}

void UPlayerHealth::AddHealth(float HealthAmount)
{
	if (HealthAmount <= 0.0f || CurrentHealth >= MaxHealth)
	{
		return;
	}

	CurrentHealth += HealthAmount;

	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}

	OnHealthChange.Broadcast(CurrentHealth, MaxHealth);
}

void UPlayerHealth::SetHealth(float HP)
{
	if (HP <= 0.0f)
	{
		return;
	}

	MaxHealth = FMath::Max(MaxHealth, 0.1f);

	const float NewHealth = FMath::Clamp(HP, 0.0f, MaxHealth);
	if (FMath::IsNearlyEqual(CurrentHealth, NewHealth))
	{
		return;
	}

	CurrentHealth = NewHealth;
	OnHealthChange.Broadcast(CurrentHealth, MaxHealth);
}

bool UPlayerHealth::ReduceHealth(float HealthAmount)
{
	bool bIsDead = false;

	if (HealthAmount <= 0.0f || CurrentHealth <= 0.0f)
	{
		return bIsDead;
	}

	CurrentHealth -= HealthAmount;

	if (CurrentHealth <= 0.0f)
	{
		CurrentHealth = 0.0f;
		bIsDead = true;
	}

	OnHealthChange.Broadcast(CurrentHealth, MaxHealth);

	return bIsDead;
}
