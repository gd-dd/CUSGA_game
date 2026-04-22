#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ACTGameMode.generated.h"

class APlayerCharacter;
class AACTPlayerController;

/**
 * 基础游戏模式类 (GameMode)
 * 职责: 配置单局游戏的核心规则和默认加载的类。
 * 说明: 在这里指定玩家进入游戏时默认使用的 Pawn、Controller 和 PlayerState。
 */
UCLASS()
class ACTGAME_API AACTGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AACTGameMode();
};
