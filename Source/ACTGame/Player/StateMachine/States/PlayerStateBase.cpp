#include "PlayerStateBase.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Character/ACTPlayerController.h"
#include "Player/Animation/PlayerAnimInstance.h"
#include "InputActionValue.h"
#include "Player/Input/InputCacheSystem.h"
#include "Player/Data/Action/ActionDataContainer.h"

void UPlayerStateBase::Init(APlayerCharacter* InCharacter, UPlayerStateMachine* InStateMachine, UInputCacheSystem* InInputCacheSystem)
{
	Character = InCharacter;
	StateMachine = InStateMachine;
	// 由于 InputCacheSystem 迁移到了 Controller，这里我们不再使用传进来的指针
	InputCacheSystem = nullptr;
}

UActionData* UPlayerStateBase::GetActionData() const
{
	if (Character)
	{
		if (UActionDataContainer* Container = Character->GetActionDataContainer())
		{
			// 遍历容器中的 Map，使用 IsChildOf 进行兼容性匹配
			// 这样即使容器里配的是 C++ 基类，而运行时是蓝图子类，也能正确匹配到数据
			for (const auto& Pair : Container->StateToDataMap)
			{
				UClass* ConfiguredClass = Pair.Key;
				if (ConfiguredClass && GetClass()->IsChildOf(ConfiguredClass))
				{
					return Pair.Value;
				}
			}
		}
	}
	return nullptr;
}

void UPlayerStateBase::EnterState()
{
	// 默认空实现，子类重写
}

void UPlayerStateBase::UpdateState(float DeltaTime)
{
	// 默认空实现，子类重写
}

void UPlayerStateBase::ExitState()
{
	// 默认空实现，子类重写
}

UPlayerAnimInstance* UPlayerStateBase::GetAnimInstance() const
{
	if (Character && Character->GetMesh())
	{
		return Cast<UPlayerAnimInstance>(Character->GetMesh()->GetAnimInstance());
	}
	return nullptr;
}

UInputCacheSystem* UPlayerStateBase::GetInputCacheSystem() const
{
	if (Character)
	{
		if (AACTPlayerController* PC = Cast<AACTPlayerController>(Character->GetController()))
		{
			return PC->InputCacheSystem;
		}
	}
	return nullptr;
}

bool UPlayerStateBase::IsInputActionTriggered(const UInputAction* Action) const
{
	if (Character && Action)
	{
		if (AACTPlayerController* PC = Cast<AACTPlayerController>(Character->GetController()))
		{
			return PC->IsInputActionTriggered(Action);
		}
	}
	return false;
}

FInputActionValue UPlayerStateBase::GetInputActionValue(const UInputAction* Action) const
{
	if (Character && Action)
	{
		if (AACTPlayerController* PC = Cast<AACTPlayerController>(Character->GetController()))
		{
			return PC->GetInputActionValue(Action);
		}
	}
	return FInputActionValue();
}

