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
	// InputCacheSystem 已迁移到 Controller，这里不再使用传入指针
	InputCacheSystem = nullptr;
}

UActionData* UPlayerStateBase::GetActionData() const
{
	if (Character)
	{
		if (UActionDataContainer* Container = Character->GetActionDataContainer())
		{
			// 使用 IsChildOf 进行兼容匹配
			// 即使配置的是 C++ 基类，也能匹配到蓝图子类状态
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

void UPlayerStateBase::Enter()
{
	// 默认空实现，子类重写
}

void UPlayerStateBase::Update(float DeltaTime)
{
	// 默认空实现，子类重写
}

void UPlayerStateBase::Exit()
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



