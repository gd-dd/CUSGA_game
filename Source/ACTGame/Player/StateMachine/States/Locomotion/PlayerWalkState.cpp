#include "PlayerWalkState.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Animation/PlayerAnimInstance.h"
#include "Player/Character/ACTPlayerController.h"
#include "Player/StateMachine/PlayerStateMachine.h"
#include "PlayerIdleState.h"
#include "PlayerTurnBackState.h"
#include "Player/Input/InputCacheSystem.h"
#include "InputActionValue.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerWalkState::EnterState()
{
	Super::EnterState();
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, TEXT("[SM] Enter State: Walk"));
	}
	
	TurnBackTimer = 0.0f; // 重置计时器
	CheckRunTimer = 0.0f; // 重置起跑计时器
	
	if (UPlayerAnimInstance* Anim = GetAnimInstance())
	{
		Anim->IsMoving = true;
	}
}

void UPlayerWalkState::UpdateState(float DeltaTime)
{
	Super::UpdateState(DeltaTime);
	
	// 注意：连击轮询已被重构到 PlayerStateMachine::StateInvoke 中集中调度
	// 这里不再主动消费 InputCacheSystem 中的攻击指令

	// 2. 检查移动输入是否结束，以及更新速度与旋转
	if (Character && StateMachine)
	{
		if (AACTPlayerController* PC = Cast<AACTPlayerController>(Character->GetController()))
		{
			if (UInputAction* MoveAction = PC->GetMoveAction())
			{
				
				FInputActionValue MoveValue = GetInputActionValue(MoveAction);
				float MoveSizeSquared = MoveValue.Get<FVector2D>().SizeSquared();
				
				if (MoveSizeSquared <= 0.01f)
				{
					// 如果没有移动输入了，回到 Idle 状态
					StateMachine->EnterState<UPlayerIdleState>();
					return;
				}
				
				// 完美复刻原蓝图逻辑：累加时间并判断是否进入奔跑状态
				if (UPlayerAnimInstance* Anim = GetAnimInstance())
				{
					if (!Anim->IsRunning)
					{
						CheckRunTimer += DeltaTime;
						// 假设 Run Start Duration 是 3.0 秒 (您可以根据项目实际值调整)
						if (CheckRunTimer >= 3.0f)
						{
							Anim->IsRunning = true;
						}
					}
				}

				// --- 3. 转身判断与平滑旋转逻辑 ---
				FRotator CurrentRotation = Character->GetActorRotation();
				FRotator TargetRotation = Character->GetTargetRotation();

				// 计算相对旋转
				FRotator RelativeRotator = UKismetMathLibrary::NormalizedDeltaRotator(TargetRotation, CurrentRotation);
				float AbsYaw = FMath::Abs(RelativeRotator.Yaw);

				// 判断是否触发急停转身 (前置条件：必须处于奔跑状态)
				bool bIsRunning = GetAnimInstance() ? GetAnimInstance()->IsRunning : false;
				if (bIsRunning && AbsYaw > 175.0f && AbsYaw < 185.0f)
				{
					TurnBackTimer += DeltaTime;
					if (TurnBackTimer > 0.1f) // 假设防手抖阈值为 0.1 秒
					{
						TurnBackTimer = 0.0f;
						if (UPlayerAnimInstance* Anim = GetAnimInstance())
						{
							StateMachine->EnterState<UPlayerTurnBackState>();
						}
						
						return;
					}
				}
				else
				{
					TurnBackTimer = 0.0f; // 不满足条件则清零

					// 使用 RInterpTo 平滑转向目标角度
					FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 10.0f);
					Character->SetActorRotation(NewRotation);
				}
				
			}
		}
	}
}

void UPlayerWalkState::ExitState()
{
	Super::ExitState();
	
	if (UPlayerAnimInstance* Anim = GetAnimInstance())
	{
		// 暂时留空，让目标状态决定要怎么处理动画变量
	}
}
