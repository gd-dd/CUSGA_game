#include "PlayerStateMachine.h"
#include "Player/Data/Action/ActionData.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Character/ACTPlayerController.h"
#include "Player/Input/InputCacheSystem.h"
#include "Tools/Log/PlayerDebug.h"
#include "Player/StateMachine/States/Locomotion/PlayerIdleState.h"
#include "Player/StateMachine/States/Locomotion/PlayerWalkState.h"
#include "Player/StateMachine/States/Combo/Attack/PlayerAttackStateBase.h"
#include "Player/StateMachine/States/Combo/Attack/Normal/PlayerAttackState_1.h"
#include "Player/StateMachine/States/Combo/Evade/PlayerEvadeState.h"
#include "Player/StateMachine/States/Combo/Attack/Special/PlayerSpecialAttackState.h"
#include "Player/StateMachine/States/PlayerStateBase.h"

UPlayerStateMachine::UPlayerStateMachine()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentState = nullptr;
}

void UPlayerStateMachine::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState)
	{
		CurrentState->Update(DeltaTime);
	}
}

void UPlayerStateMachine::EnterState(UClass* StateClass)
{
	if (!StateClass || !StateClass->IsChildOf(UPlayerStateBase::StaticClass()))
	{
		return;
	}

	if (CurrentState && CurrentState->GetClass() == StateClass)
	{
		return;
	}

	UPlayerStateBase* NextState = LoadState(StateClass);
	if (!NextState)
	{
		return;
	}

	ExitCurrentState();
	EnterCurrentState(NextState);
}

UPlayerStateBase* UPlayerStateMachine::LoadState(UClass* StateClass)
{
	if (!StateClass || !StateClass->IsChildOf(UPlayerStateBase::StaticClass()))
	{
		return nullptr;
	}

	if (UPlayerStateBase** FoundState = StateDic.Find(StateClass))
	{
		return *FoundState;
	}

	// 如果状态未创建，则实例化并注入依赖
	UPlayerStateBase* NewState = NewObject<UPlayerStateBase>(this, StateClass);
	if (!NewState)
	{
		return nullptr;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	NewState->Init(PlayerCharacter, this, nullptr);
	StateDic.Add(StateClass, NewState);
	return NewState;
}

void UPlayerStateMachine::EnterCurrentState(UPlayerStateBase* NewState)
{
	CurrentState = NewState;
	if (!CurrentState)
	{
		return;
	}

	// 调试信息只输出到日志
	DEBUG_LOG(TEXT("[StateMachine] EnterState: %s"), *CurrentState->GetClass()->GetName());
	CurrentState->Enter();
}

void UPlayerStateMachine::ExitCurrentState()
{
	if (!CurrentState)
	{
		return;
	}

	CurrentState->Exit();
	CurrentState = nullptr;
}

void UPlayerStateMachine::Stop()
{
	ExitCurrentState();
	StateDic.Empty();
}

void UPlayerStateMachine::StateInvoke(EInputType InputType)
{
	if (InputType == EInputType::NormalAttack)
	{
		UPlayerStateBase* CurrentStateObj = CurrentState;
		if (CurrentStateObj)
		{
			UClass* CurrentClass = CurrentStateObj->GetClass();
			
			// 1. 非攻击状态下，直接进入第一段攻击
			if (CurrentClass->IsChildOf(UPlayerIdleState::StaticClass()) || CurrentClass->IsChildOf(UPlayerWalkState::StaticClass()))
			{
				if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
				{
					if (AACTPlayerController* PC = Cast<AACTPlayerController>(PlayerCharacter->GetController()))
					{
						if (PC->InputCacheSystem)
						{
							PC->InputCacheSystem->ClearCache();
						}
					}
				}
				EnterState(UPlayerAttackState_1::StaticClass());
				return;
			}
			// 2. 攻击状态处于退出窗口时，重新进入第一段攻击
			else if (CurrentClass->IsChildOf(UPlayerAttackStateBase::StaticClass()))
			{
				UPlayerAttackStateBase* AttackState = Cast<UPlayerAttackStateBase>(CurrentStateObj);
				if (AttackState && AttackState->GetCanMontageExit())
				{
					if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
					{
						if (AACTPlayerController* PC = Cast<AACTPlayerController>(PlayerCharacter->GetController()))
						{
							if (PC->InputCacheSystem)
							{
								PC->InputCacheSystem->ClearCache();
							}
						}
					}
					EnterState(UPlayerAttackState_1::StaticClass());
					return;
				}
			}
			
			// 其他情况只保留输入缓存，等待 ComboUpdate 消费
		}
	}
	else if (InputType == EInputType::Evade)
	{
		UPlayerStateBase* CurrentStateObj = CurrentState;
		if (CurrentStateObj)
		{
			// 闪避优先级最高，清空缓存并强制进入闪避状态
			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
			{
				if (AACTPlayerController* PC = Cast<AACTPlayerController>(PlayerCharacter->GetController()))
				{
					if (PC->InputCacheSystem)
					{
						PC->InputCacheSystem->ClearCache();
					}
				}
			}
			EnterState(UPlayerEvadeState::StaticClass());
		}
	}
	else if (InputType == EInputType::SpecialAttack)
	{
		UPlayerStateBase* CurrentStateObj = CurrentState;
		if (CurrentStateObj)
		{
			UClass* CurrentClass = CurrentStateObj->GetClass();
			
			// Idle 或 Walk 状态下，清空缓存并进入特殊技状态
			if (CurrentClass->IsChildOf(UPlayerIdleState::StaticClass()) || CurrentClass->IsChildOf(UPlayerWalkState::StaticClass()))
			{
				if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
				{
					if (AACTPlayerController* PC = Cast<AACTPlayerController>(PlayerCharacter->GetController()))
					{
						if (PC->InputCacheSystem)
						{
							PC->InputCacheSystem->ClearCache();
						}
					}
				}
				EnterState(UPlayerSpecialAttackState::StaticClass());
			}
			// 如果在攻击状态中，且处于退出窗口（CanMontageExit == true），允许使用特殊技打断后摇
			else if (CurrentClass->IsChildOf(UPlayerAttackStateBase::StaticClass()))
			{
				UPlayerAttackStateBase* AttackState = Cast<UPlayerAttackStateBase>(CurrentStateObj);
				if (AttackState && AttackState->GetCanMontageExit())
				{
					if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
					{
						if (AACTPlayerController* PC = Cast<AACTPlayerController>(PlayerCharacter->GetController()))
						{
							if (PC->InputCacheSystem)
							{
								PC->InputCacheSystem->ClearCache();
							}
						}
					}
					EnterState(UPlayerSpecialAttackState::StaticClass());
				}
			}
		}
	}
}

void UPlayerStateMachine::ComboUpdate()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
	{
		if (AACTPlayerController* PC = Cast<AACTPlayerController>(PlayerCharacter->GetController()))
		{
			if (UInputCacheSystem* CacheSys = PC->InputCacheSystem)
			{
				EInputType CachedInput;
				// 获取并消费一个缓存输入
				if (CacheSys->GetCache(CachedInput))
				{
					// 如果拿到有效指令，则执行状态重入派发
					StateReInvoke(CachedInput);
					// 成功处理后清空队列，防止重复消费
					CacheSys->ClearCache();
				}
			}
		}
	}
}

void UPlayerStateMachine::StateReInvoke(EInputType InputType)
{
	UPlayerStateBase* CurrentStateObj = CurrentState;
	if (!CurrentStateObj) return;
	
    UClass* CurrentClass = CurrentStateObj->GetClass();

	if (InputType == EInputType::NormalAttack)
	{
		if (UActionData* CurrentActionData = CurrentStateObj->GetActionData())
		{
			if (CurrentActionData->NextComboState)
			{
				EnterState(CurrentActionData->NextComboState.Get());
				return;
			}
		}
	}
	else if (InputType == EInputType::SpecialAttack)
	{
		// 允许在任何普通攻击连段期间，派生切入特殊攻击
		if (CurrentClass->IsChildOf(UPlayerAttackStateBase::StaticClass()))
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Yellow, TEXT("[SM] Transitioning: Attack -> SpecialAttack"));
			EnterState(UPlayerSpecialAttackState::StaticClass());
		}
	}
}


