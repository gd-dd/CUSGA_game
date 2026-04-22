#include "PlayerSpecialAttackState.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Input/InputCacheSystem.h"
#include "Player/StateMachine/PlayerStateMachine.h"
#include "Player/StateMachine/States/Locomotion/PlayerIdleState.h"
#include "Player/Data/Action/ActionData.h"

void UPlayerSpecialAttackState::EnterState()
{
	Super::EnterState();
	SetCanMontageExit(false);

	if (UActionData* MyData = GetActionData())
	{
		AttackMontage = MyData->ActionMontage;
	}

	if (Character && AttackMontage)
	{
		// 播放特殊攻击蒙太奇
		Character->PlayCombatMontage(AttackMontage);
		
		// 绑定动画结束回调
		if (UAnimInstance* AnimInst = Character->GetMesh()->GetAnimInstance())
		{
			AnimInst->OnMontageEnded.AddDynamic(this, &UPlayerSpecialAttackState::OnMontageEnded);
		}
	}
}

void UPlayerSpecialAttackState::UpdateState(float DeltaTime)
{
	Super::UpdateState(DeltaTime);
}

void UPlayerSpecialAttackState::ExitState()
{
	Super::ExitState();
	SetCanMontageExit(false);
	
	// 解绑回调以防内存泄漏
	if (Character)
	{
		if (UAnimInstance* AnimInst = Character->GetMesh()->GetAnimInstance())
		{
			AnimInst->OnMontageEnded.RemoveDynamic(this, &UPlayerSpecialAttackState::OnMontageEnded);
		}
	}
}

void UPlayerSpecialAttackState::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackMontage && !bInterrupted)
	{
		if (StateMachine)
		{
			StateMachine->EnterState(UPlayerIdleState::StaticClass());
		}
	}
}
