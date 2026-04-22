// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/StateMachine/States/PlayerStateBase.h"
#include "PlayerTurnBackState.generated.h"

/**
 * 独立转身状态
 * 负责处理超过135度急停转身的逻辑，根据起步脚播放对应的转身动画，结束后切回Idle或Walk
 */
UCLASS()
class ACTGAME_API UPlayerTurnBackState : public UPlayerStateBase
{
	GENERATED_BODY()

public:
	virtual void EnterState() override;
	virtual void UpdateState(float DeltaTime) override;
	virtual void ExitState() override;

private:
	// 动画播放结束的回调绑定 (如果使用Montage驱动转身)
	// UFUNCTION()
	// void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
