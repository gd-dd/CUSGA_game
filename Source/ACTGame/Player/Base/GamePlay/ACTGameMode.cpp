#include "ACTGameMode.h"
#include "Player/Character/ACTPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Player/Character/PlayerCharacter.h"
#include "ACTPlayerState.h"
#include "Player/UI/UIManager.h"

AACTGameMode::AACTGameMode()
{
	// 配置默认玩家控制器为 AACTPlayerController
	PlayerControllerClass = AACTPlayerController::StaticClass();

	// 配置默认 Pawn 为 APlayerCharacter
	DefaultPawnClass = APlayerCharacter::StaticClass();

	// 设置默认的 PlayerState 类
	PlayerStateClass = AACTPlayerState::StaticClass();

	// 设置默认 HUD 为 UIManager
	HUDClass = AUIManager::StaticClass();
}
