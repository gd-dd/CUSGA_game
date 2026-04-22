#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IState.generated.h"

UINTERFACE(MinimalAPI)
class UState : public UInterface
{
	GENERATED_BODY()
};

/**
 * 状态机状态接口 (IState)
 * 职责: 定义所有具体动作状态（跑、跳、攻击等）必须遵守的生命周期契约。
 * 说明: 通过纯虚函数强制子类实现进入、更新和退出逻辑，实现状态模式。
 */
class ACTGAME_API IState
{
	GENERATED_BODY()

public:
	virtual void EnterState() = 0;
	virtual void UpdateState(float DeltaTime) = 0;
	virtual void ExitState() = 0;
};
