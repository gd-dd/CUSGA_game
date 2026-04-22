#include "PlayerAttackState_3.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Input/InputCacheSystem.h"
#include "Player/StateMachine/PlayerStateMachine.h"
#include "Player/StateMachine/States/Combo/Attack/Normal/PlayerAttackState_4.h"
#include "Player/StateMachine/States/Locomotion/PlayerIdleState.h"
#include "Player/Data/Action/ActionData.h"

UPlayerAttackState_3::UPlayerAttackState_3()
{
}

void UPlayerAttackState_3::EnterState()
{
	Super::EnterState();
	SetCanMontageExit(false);
	
	if (UActionData* MyData = GetActionData())
	{
		AttackMontage = MyData->ActionMontage;
	}

	if (Character && AttackMontage)
	{
		Character->PlayCombatMontage(AttackMontage);
		
		if (UAnimInstance* AnimInst = Character->GetMesh()->GetAnimInstance())
		{
			AnimInst->OnMontageEnded.AddDynamic(this, &UPlayerAttackState_3::OnMontageEnded);
		}
	}
}

void UPlayerAttackState_3::UpdateState(float DeltaTime)
{
	Super::UpdateState(DeltaTime);

	// 注意：此处不再轮询缓存。
	// 原蓝图中，连击的触发完全由动画通知事件（ANS_AttackCombo -> ComboUpdate()）驱动。
	// 退出窗口重入则由 StateInvoke 配合 CanMontageExit 判断处理。
}

void UPlayerAttackState_3::ExitState()
{
	Super::ExitState();
	SetCanMontageExit(false);
	
	if (Character)
	{
		if (UAnimInstance* AnimInst = Character->GetMesh()->GetAnimInstance())
		{
			AnimInst->OnMontageEnded.RemoveDynamic(this, &UPlayerAttackState_3::OnMontageEnded);
		}
	}
}

void UPlayerAttackState_3::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackMontage && !bInterrupted)
	{
		if (StateMachine)
		{
			StateMachine->EnterState(UPlayerIdleState::StaticClass());
		}
	}
}
