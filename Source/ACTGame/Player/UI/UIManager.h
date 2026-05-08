#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UIManager.generated.h"

class UPlayerHud;

UCLASS()
class ACTGAME_API AUIManager : public AHUD
{
	GENERATED_BODY()

public:
	AUIManager();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Player|UI")
	void InitializeUI();

	UFUNCTION(BlueprintPure, Category = "Player|UI")
	UPlayerHud* GetPlayerHud() const { return PlayerHudInstance; }

	void DestroyUI();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|UI")
	TSubclassOf<UPlayerHud> PlayerHudClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Player|UI")
	TObjectPtr<UPlayerHud> PlayerHudInstance = nullptr;
};
