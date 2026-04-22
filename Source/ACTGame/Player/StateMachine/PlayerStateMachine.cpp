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
		CurrentState->UpdateState(DeltaTime);
	}
}

void UPlayerStateMachine::Stop()
{
	if (CurrentState)
	{
		CurrentState->ExitState();
		CurrentState = nullptr;
	}
}

void UPlayerStateMachine::EnterState(UClass* StateClass)
{
	if (!StateClass) return;

	if (!StateDic.Contains(StateClass))
	{
		// 如果字典中没有该状态，则实例化并进行依赖注入
		UPlayerStateBase* NewState = NewObject<UPlayerStateBase>(this, StateClass);
		if (NewState)
		{
			// 获取相关的组件引用
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
			UInputCacheSystem* InputCache = nullptr;
			
			NewState->Init(PlayerCharacter, this, InputCache);
			StateDic.Add(StateClass, NewState);
		}
	}

	if (UPlayerStateBase** FoundState = StateDic.Find(StateClass))
	{
		if (CurrentState)
		{
			CurrentState->ExitState();
		}

		CurrentState = *FoundState;
		
		if (CurrentState)
		{
			// 调试信息：只在后台输出，不占用屏幕
			DEBUG_LOG(TEXT("[StateMachine] EnterState: %s"), *StateClass->GetName());
			CurrentState->EnterState();
		}
	}
}

void UPlayerStateMachine::StateInvoke(EInputType InputType)
{
	if (InputType == EInputType::NormalAttack)
	{
		UPlayerStateBase* CurrentStateObj = CurrentState;
		if (CurrentStateObj)
		{
			UClass* CurrentClass = CurrentStateObj->GetClass();
			
			// 1. 如果在非攻击状态（Idle 或 Walk），直接进入第一段攻击
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
			// 2. 如果在攻击状态中，且处于退出窗口（CanMontageExit == true），按下攻击键意味着掉连击并重新开始第一段
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
			
			// 其他情况（攻击中且没到退出窗口）：按键已被存入缓存，无需操作，等待 ComboUpdate 取出即可
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
			
			// 当当前状态为 Idle 或 Walk 时，清空缓存并进入特殊技状态
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
					// 如果拿到了有效指令，执行状态重入派发
					StateReInvoke(CachedInput);
					// 成功处理后，清空队列，防止“吃指令”或“连放技能”
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
