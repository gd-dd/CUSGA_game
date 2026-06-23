#pragma once

#include "CoreMinimal.h"
#include "Player/StateMachine/States/PlayerStateBase.h"
#include "PlayerWalkState.generated.h"

/**
 * 锟斤拷锟斤拷锟斤拷锟阶刺?
 */
UCLASS()	
class ACTGAME_API UPlayerWalkState : public UPlayerStateBase
{
	GENERATED_BODY()

public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;

private:
	// 杞韩闃叉墜鎶栬鏃跺櫒
	float TurnBackTimer = 0.0f;
	// 璺戞鍚姩璁℃椂鍣?
	float CheckRunTimer = 0.0f;
};

