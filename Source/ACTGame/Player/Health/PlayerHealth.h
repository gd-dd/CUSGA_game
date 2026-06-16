#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerHealth.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangeSignature, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeathSignature, bool, bIsDead);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTGAME_API UPlayerHealth : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerHealth();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Player|Health")
	void AddHealth(float HealthAmount);

	UFUNCTION(BlueprintCallable, Category = "Player|Health")
	void SetHealth(float HP);

	UFUNCTION(BlueprintCallable, Category = "Player|Health")
	void ReduceHealth(float HealthAmount);

	UFUNCTION(BlueprintPure, Category = "Player|Health")
	float GetHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "Player|Health")
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "Player|Health")
	float GetMaxHealth() const { return MaxHealth; }

public:
	UPROPERTY(BlueprintAssignable, Category = "Player|Health")
	FOnHealthChangeSignature OnHealthChange;

	UPROPERTY(BlueprintAssignable, Category = "Player|Health")
	FOnPlayerDeathSignature OnPlayerDeath;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Health", meta = (ClampMin = "0.1"))
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Health")
	float CurrentHealth = 100.0f;
};
