#include "PlayerIdleState.h"
#include "Player/StateMachine/PlayerStateMachine.h"
#include "Player/Input/InputCacheSystem.h"
#include "Player/Animation/PlayerAnimInstance.h"
#include "PlayerWalkState.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Character/ACTPlayerController.h"
#include "InputActionValue.h"

void UPlayerIdleState::Enter()
{
	Super::Enter();
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, TEXT("[SM] Enter State: Idle"));
	}
	
	if (UPlayerAnimInstance* Anim = GetAnimInstance())
	{
		Anim->IsMoving = false;
		Anim->IsRunning = false;
	}
}

void UPlayerIdleState::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
	
	
	
	// 连击轮询已被重构到 PlayerStateMachine::StateInvoke 中集中处理
	// 这里不再主动消费 InputCacheSystem 中的攻击指令

	// 2. 检查移动输入，如果不为 0 则进入 Walk 状态
	if (Character && StateMachine)
	{
		if (AACTPlayerController* PC = Cast<AACTPlayerController>(Character->GetController()))
		{
			if (UInputAction* MoveAction = PC->GetMoveAction())
			{
				FInputActionValue MoveValue = GetInputActionValue(MoveAction);
				// 增强输入的值通常是 Vector2D，这里取其平方长度
				if (MoveValue.Get<FVector2D>().SizeSquared() > 0.01f)
				{
					StateMachine->EnterState<UPlayerWalkState>();
					return;
				}
			}
		}
	}
}

void UPlayerIdleState::Exit()
{
	Super::Exit();
	// TODO: 实现退出空闲状态的逻辑
}


