#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameManager.generated.h"

class USaveActGame;

UCLASS()
class ACTGAME_API UGameManager : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SaveGame")
	USaveActGame* SaveGameRef = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SaveGame")
	bool bIsSaving = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SaveGame")
	TArray<FString> SaveSlotNames;

private:
	void InitializeSaveSlots();
	void CreateDefaultSaveGame();
};
