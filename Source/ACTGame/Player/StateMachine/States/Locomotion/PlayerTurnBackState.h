// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/StateMachine/States/PlayerStateBase.h"
#include "PlayerTurnBackState.generated.h"

/**
 * 独立转身状态
 * 负责处理急停转身逻辑，动画结束后切回 Idle 或 Walk。
 */
UCLASS()
class ACTGAME_API UPlayerTurnBackState : public UPlayerStateBase
{
	GENERATED_BODY()

public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;

private:
	// 动画结束回调预留
	// UFUNCTION()
	// void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};

