#pragma once

#include "CoreMinimal.h"
#include "Player/StateMachine/IState.h"
#include "PlayerStateBase.generated.h"

class APlayerCharacter;
class UPlayerStateMachine;
class UInputCacheSystem;
struct FInputActionValue;
class UPlayerAnimInstance;

/**
 * 玩家具体状态基类 (PlayerStateBase)
 * 职责: 为所有具体玩家状态提供公共方法和共享引用。
 * 说明:
 * 1. 继承自 UObject，并实现 IState 接口。
 * 2. 持有 Character、StateMachine 和 InputCacheSystem 的引用。
 * 3. 提供 GetAnimInstance() 便于子类向动画蓝图推送变量。
 */
UCLASS(Blueprintable, BlueprintType)
class ACTGAME_API UPlayerStateBase : public UObject, public IState
{
	GENERATED_BODY()

public:
	virtual void Init(APlayerCharacter* InCharacter, UPlayerStateMachine* InStateMachine, UInputCacheSystem* InInputCacheSystem);

	// 获取当前状态绑定的动作数据
	class UActionData* GetActionData() const;

	//~ Begin IState Interface
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;
	//~ End IState Interface

protected:
	// 获取玩家动画实例
	UPlayerAnimInstance* GetAnimInstance() const;

	// 获取输入缓存系统
	UInputCacheSystem* GetInputCacheSystem() const;

	// 检查指定输入动作当前是否被触发
	bool IsInputActionTriggered(const class UInputAction* Action) const;

	// 获取指定输入动作的当前值
	struct FInputActionValue GetInputActionValue(const class UInputAction* Action) const;

	UPROPERTY()
	APlayerCharacter* Character;

	UPROPERTY()
	UPlayerStateMachine* StateMachine;

	UPROPERTY()
	UInputCacheSystem* InputCacheSystem;
};

