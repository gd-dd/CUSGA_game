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
 * 职责: 定义所有具体动作状态必须遵守的生命周期契约。
 * 说明: 通过纯虚函数强制子类实现进入、更新和退出逻辑。
 */
class ACTGAME_API IState
{
	GENERATED_BODY()

public:
	virtual void Enter() = 0;
	virtual void Update(float DeltaTime) = 0;
	virtual void Exit() = 0;
};

