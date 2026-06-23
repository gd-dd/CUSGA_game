#pragma once

#include "CoreMinimal.h"
#include "Player/StateMachine/States/PlayerStateBase.h"
#include "PlayerIdleState.generated.h"

/**
 * 玩家空闲状态
 */
UCLASS()
class ACTGAME_API UPlayerIdleState : public UPlayerStateBase
{
	GENERATED_BODY()

public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;
};

