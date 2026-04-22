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

void UPlayerTurnBackState::EnterState()
{
	Super::EnterState();

	// 设置动画实例变量，触发 AnimGraph 的转身节点
	if (UPlayerAnimInstance* Anim = GetAnimInstance())
	{
		Anim->IsTurnBack = true;
	}
}

void UPlayerTurnBackState::UpdateState(float DeltaTime)
{
	Super::UpdateState(DeltaTime);
	
	if (!Character || !StateMachine) return;

	// 1. 处理角色旋转
	// 因为进入 TurnBack 的条件是角度在 175~185 之间，且日常插值已在 Walk 状态处理
	// 所以这里不需要再做任何代码层的旋转插值，完全交由动画根运动处理即可

	// 2. 检查转身是否完成
	// 退出机制：当动画蓝图的退出函数将 IsTurnBack 设置为 false 时，说明动画播放结束
	bool bIsTurnBackFinished = false;
	if (UPlayerAnimInstance* Anim = GetAnimInstance())
	{
		// 如果 AnimGraph 把 IsTurnBack 改成了 false，说明动画状态机已经退出了转身节点
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

void UPlayerTurnBackState::ExitState()
{
	Super::ExitState();

	// 重置动画实例的转身变量，同时把移动变量也重置一下
	// 因为从 TurnBack 出来，要么切到 Idle，要么切回 Walk
	// 在进入新状态时，它们会根据当时的输入重新被设置为 true 或 false
	if (UPlayerAnimInstance* Anim = GetAnimInstance())
	{
		Anim->IsTurnBack = false;
		Anim->IsMoving = false;
		Anim->IsRunning = false;
	}
}
