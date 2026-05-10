#include "PlayerHud.h"

#include "Player/Character/PlayerCharacter.h"
#include "Player/Health/PlayerHealth.h"

void UPlayerHud::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
	{
		InitializePlayerHud(PlayerCharacter);
	}
}

void UPlayerHud::InitializePlayerHud(APlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	UPlayerHealth* PlayerHealth = PlayerCharacter->GetPlayerHealth();
	if (!PlayerHealth)
	{
		return;
	}

	BindPlayerHealth(PlayerHealth);
	UpdateHealth(PlayerHealth->GetCurrentHealth(), PlayerHealth->GetMaxHealth());
}

void UPlayerHud::UpdateHealth_Implementation(float CurrentHealth, float MaxHealth)
{
}

void UPlayerHud::BindPlayerHealth(UPlayerHealth* NewPlayerHealth)
{
	if (!NewPlayerHealth)
	{
		return;
	}

	NewPlayerHealth->OnHealthChange.RemoveDynamic(this, &UPlayerHud::UpdateHealth);
	NewPlayerHealth->OnHealthChange.AddDynamic(this, &UPlayerHud::UpdateHealth);
}
