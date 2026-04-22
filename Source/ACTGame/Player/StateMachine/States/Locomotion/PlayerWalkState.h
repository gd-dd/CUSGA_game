#pragma once

#include "CoreMinimal.h"
#include "Player/StateMachine/States/PlayerStateBase.h"
#include "PlayerWalkState.generated.h"

/**
 * �������״̬
 */
UCLASS()	
class ACTGAME_API UPlayerWalkState : public UPlayerStateBase
{
	GENERATED_BODY()

public:
	virtual void EnterState() override;
	virtual void UpdateState(float DeltaTime) override;
	virtual void ExitState() override;

private:
	// 转身防手抖计时器
	float TurnBackTimer = 0.0f;
	// 跑步启动计时器
	float CheckRunTimer = 0.0f;
};
