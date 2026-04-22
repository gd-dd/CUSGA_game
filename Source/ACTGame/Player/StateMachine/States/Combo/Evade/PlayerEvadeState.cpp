#include "PlayerEvadeState.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Animation/PlayerAnimInstance.h"
#include "Player/Character/ACTPlayerController.h"
#include "Player/StateMachine/PlayerStateMachine.h"
#include "Player/StateMachine/States/Locomotion/PlayerIdleState.h"
#include "Player/Input/InputCacheSystem.h"
#include "InputActionValue.h"
#include "Player/Data/Action/ActionData.h"

UPlayerEvadeState::UPlayerEvadeState()
{
}

void UPlayerEvadeState::EnterState()
{
	Super::EnterState();

	if (!Character || !StateMachine) return;

	// 获取动作数据
	if (UActionData* MyData = GetActionData())
	{
		AM_Evade_Front = MyData->ActionMontage;
		if (MyData->ExtraMontages.Num() > 0)
		{
			AM_Evade_Back = MyData->ExtraMontages[0];
		}
	}

	// 进入闪避时，必须关闭原有的移动和奔跑动画状态，防止底层状态机动画残留
	if (UPlayerAnimInstance* AnimInstance = GetAnimInstance())
	{
		AnimInstance->IsMoving = false;
		AnimInstance->IsRunning = false;
	}

	bool bHasMoveInput = false;
	bShouldRotate = false;

	// 检查是否有方向输入
	if (AACTPlayerController* PC = Cast<AACTPlayerController>(Character->GetController()))
	{
		if (UInputAction* MoveAction = PC->GetMoveAction())
		{
			FInputActionValue MoveValue = GetInputActionValue(MoveAction);
			if (!MoveValue.Get<FVector2D>().IsNearlyZero())
			{
				bHasMoveInput = true;
				bShouldRotate = true; 
			}
		}
	}

	UAnimMontage* MontageToPlay = nullptr;

	// 有输入时播放前向闪避并对齐朝向
	if (bHasMoveInput)
	{
		MontageToPlay = AM_Evade_Front;
		Character->SetActorRotation(Character->GetTargetRotation());
	}
	else
	{
		MontageToPlay = AM_Evade_Back;
	}

	// 播放动画并绑定结束事件
	if (MontageToPlay)
	{
		Character->PlayCombatMontage(MontageToPlay);
		if (UAnimInstance* AnimInstance = GetAnimInstance())
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &UPlayerEvadeState::OnMontageEnded);
		}
	}
	else
	{
		// 容错处理：如果蒙太奇为空（加载失败），立刻切回 Idle 防止状态机卡死
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Evade Montage is NULL!"));
		StateMachine->EnterState<UPlayerIdleState>();
	}
}

void UPlayerEvadeState::UpdateState(float DeltaTime)
{
	Super::UpdateState(DeltaTime);

	// 根据输入平滑转向
	if (bShouldRotate && Character)
	{
		FRotator CurrentRotation = Character->GetActorRotation();
		FRotator TargetRotation = Character->GetTargetRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, CharacterRotateSpeed);
		
		Character->SetActorRotation(NewRotation);
	}
}

void UPlayerEvadeState::ExitState()
{
	Super::ExitState();

	// 清理委托
	if (UAnimInstance* AnimInstance = GetAnimInstance())
	{
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &UPlayerEvadeState::OnMontageEnded);
	}
}

void UPlayerEvadeState::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 动画结束回退到待机
	if (StateMachine)
	{
		StateMachine->EnterState<UPlayerIdleState>();
	}
}
