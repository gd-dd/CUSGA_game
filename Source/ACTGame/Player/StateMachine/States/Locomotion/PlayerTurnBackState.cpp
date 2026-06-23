// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/StateMachine/States/Locomotion/PlayerTurnBackState.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Animation/PlayerAnimInstance.h"
#include "Player/Character/ACTPlayerController.h"
#include "Player/StateMachine/PlayerStateMachine.h"
#include "Player/StateMachine/States/Locomotion/PlayerIdleState.h"
#include "Player/StateMachine/States/Locomotion/PlayerWalkState.h"
#include "Kismet/KismetMathLibrary.h"
#include "InputActionValue.h"

void UPlayerTurnBackState::Enter()
{
	Super::Enter();

	// 设置动画实例变量，触发 AnimGraph 的转身节点
	if (UPlayerAnimInstance* Anim = GetAnimInstance())
	{
		Anim->IsTurnBack = true;
	}
}

void UPlayerTurnBackState::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
	
	if (!Character || !StateMachine) return;

	// 1. 处理角色旋转
	// 进入 TurnBack 时已经满足转身角度条件
	// 这里不再额外做旋转插值，交给动画根运动处理

	// 2. 检查转身是否完成
	// 当动画蓝图把 IsTurnBack 设为 false 时，说明转身播放结束
	bool bIsTurnBackFinished = false;
	if (UPlayerAnimInstance* Anim = GetAnimInstance())
	{
		// 如果 IsTurnBack 已经变为 false，说明动画状态机已退出转身节点
		bIsTurnBackFinished = !Anim->IsTurnBack;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Yellow, TEXT("[SM] Enter State: TurnBack"));
		}
	}

	if (bIsTurnBackFinished)
	{
		// 检查当前是否有移动输入
		bool bHasMoveInput = false;
		if (AACTPlayerController* PC = Cast<AACTPlayerController>(Character->GetController()))
		{
			if (UInputAction* MoveAction = PC->GetMoveAction())
			{
				float MoveSizeSquared = PC->GetInputActionValue(MoveAction).Get<FVector2D>().SizeSquared();
				if (MoveSizeSquared > 0.01f)
				{
					bHasMoveInput = true;
				}
			}
		}

		if (bHasMoveInput)
		{
			StateMachine->EnterState<UPlayerWalkState>();
		}
		else
		{
			StateMachine->EnterState<UPlayerIdleState>();
		}
	}
}

void UPlayerTurnBackState::Exit()
{
	Super::Exit();

	// 重置动画实例变量
	// 离开 TurnBack 后，新状态会根据输入重新设置这些值
	if (UPlayerAnimInstance* Anim = GetAnimInstance())
	{
		Anim->IsTurnBack = false;
		Anim->IsMoving = false;
		Anim->IsRunning = false;
	}
}


