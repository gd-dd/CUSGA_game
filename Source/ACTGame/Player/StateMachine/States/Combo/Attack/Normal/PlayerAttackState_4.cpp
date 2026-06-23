#include "PlayerAttackState_4.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Input/InputCacheSystem.h"
#include "Player/StateMachine/PlayerStateMachine.h"
#include "Player/StateMachine/States/Combo/Attack/Normal/PlayerAttackState_1.h"
#include "Player/StateMachine/States/Locomotion/PlayerIdleState.h"
#include "Player/Data/Action/ActionData.h"

UPlayerAttackState_4::UPlayerAttackState_4()
{
}

void UPlayerAttackState_4::Enter()
{
	Super::Enter();
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
			AnimInst->OnMontageEnded.AddDynamic(this, &UPlayerAttackState_4::OnMontageEnded);
		}
	}
}

void UPlayerAttackState_4::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	// 这里不再轮询缓存
	// 连击由动画通知事件驱动
	// 退出窗口重入由 StateInvoke 配合 CanMontageExit 处理
}

void UPlayerAttackState_4::Exit()
{
	Super::Exit();
	SetCanMontageExit(false);
	
	if (Character)
	{
		if (UAnimInstance* AnimInst = Character->GetMesh()->GetAnimInstance())
		{
			AnimInst->OnMontageEnded.RemoveDynamic(this, &UPlayerAttackState_4::OnMontageEnded);
		}
	}
}

void UPlayerAttackState_4::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackMontage && !bInterrupted)
	{
		if (StateMachine)
		{
			StateMachine->EnterState(UPlayerIdleState::StaticClass());
		}
	}
}


