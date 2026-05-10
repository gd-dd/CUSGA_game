#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHud.generated.h"

class APlayerCharacter;
class UPlayerHealth;

UCLASS()
class ACTGAME_API UPlayerHud : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Player|UI")
	void InitializePlayerHud(APlayerCharacter* PlayerCharacter);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Player|UI")
	void UpdateHealth(float CurrentHealth, float MaxHealth);

	virtual void UpdateHealth_Implementation(float CurrentHealth, float MaxHealth);

	void BindPlayerHealth(UPlayerHealth* NewPlayerHealth);
};
