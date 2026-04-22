#include "PlayerAttackState_1.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Input/InputCacheSystem.h"
#include "Player/StateMachine/PlayerStateMachine.h"
#include "Player/StateMachine/States/Combo/Attack/Normal/PlayerAttackState_2.h"
#include "Player/StateMachine/States/Locomotion/PlayerIdleState.h"
#include "Player/Data/Action/ActionData.h"

UPlayerAttackState_1::UPlayerAttackState_1()
{
}

void UPlayerAttackState_1::EnterState()
{
	Super::EnterState();
	SetCanMontageExit(false);
	
	if (UActionData* MyData = GetActionData())
	{
		AttackMontage = MyData->ActionMontage;
	}

	if (Character && AttackMontage)
	{
		// 播放蒙太奇
		Character->PlayCombatMontage(AttackMontage);
		
		// 绑定动画结束回调
		if (UAnimInstance* AnimInst = Character->GetMesh()->GetAnimInstance())
		{
			AnimInst->OnMontageEnded.AddDynamic(this, &UPlayerAttackState_1::OnMontageEnded);
		}
	}
}

void UPlayerAttackState_1::UpdateState(float DeltaTime)
{
	Super::UpdateState(DeltaTime);

	// 注意：此处不再轮询缓存。
	// 原蓝图中，连击的触发完全由动画通知事件（ANS_AttackCombo -> ComboUpdate()）驱动。
	// 退出窗口重入则由 StateInvoke 配合 CanMontageExit 判断处理。
}

void UPlayerAttackState_1::ExitState()
{
	Super::ExitState();
	SetCanMontageExit(false);
	
	// 解绑回调以防内存泄漏
	if (Character)
	{
		if (UAnimInstance* AnimInst = Character->GetMesh()->GetAnimInstance())
		{
			AnimInst->OnMontageEnded.RemoveDynamic(this, &UPlayerAttackState_1::OnMontageEnded);
		}
	}
}

void UPlayerAttackState_1::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackMontage && !bInterrupted)
	{
		if (StateMachine)
		{
			StateMachine->EnterState(UPlayerIdleState::StaticClass());
		}
	}
}
