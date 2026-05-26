#pragma once

#include "CoreMinimal.h"
#include "PlayerData.generated.h"

USTRUCT(BlueprintType)
struct ACTGAME_API FPlayerData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "SaveGame")
	float HP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "SaveGame")
	FTransform Transform = FTransform::Identity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "SaveGame")
	FRotator CameraRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "SaveGame")
	bool bHasSavedData = false;
};
