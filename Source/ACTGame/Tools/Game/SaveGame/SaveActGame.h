#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Tools/Game/PlayerData.h"
#include "SaveActGame.generated.h"

UCLASS()
class ACTGAME_API USaveActGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "SaveGame")
	FPlayerData PlayerData;
};
