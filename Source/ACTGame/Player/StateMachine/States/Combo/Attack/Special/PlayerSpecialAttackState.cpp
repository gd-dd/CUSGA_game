#include "PlayerSpecialAttackState.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Input/InputCacheSystem.h"
#include "Player/StateMachine/PlayerStateMachine.h"
#include "Player/StateMachine/States/Locomotion/PlayerIdleState.h"
#include "Player/Data/Action/ActionData.h"

void UPlayerSpecialAttackState::Enter()
{
	Super::Enter();
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

void UPlayerSpecialAttackState::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
}

void UPlayerSpecialAttackState::Exit()
{
	Super::Exit();
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


