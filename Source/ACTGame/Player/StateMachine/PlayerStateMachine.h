#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/Input/InputCacheSystem.h" // 包含 EInputType 枚举
#include "PlayerStateMachine.generated.h"

// 前向声明
class UPlayerStateBase;
class APlayerCharacter;

/**
 * 玩家状态机组件 (PlayerStateMachine)
 * 职责: 管理和调度玩家所有的动作状态。
 * 说明:
 * 1. 作为一个组件挂载在 PlayerCharacter 上。
 * 2. 使用泛型模板 EnterState<T>() 进行高效、类型安全的状态切换。
 * 3. 内部使用 TMap 实现状态对象池，避免频繁实例化造成的 GC 压力。
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTGAME_API UPlayerStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerStateMachine();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * 强制切换到指定类型的状态（基于类指针），主要供泛型内部或外部统一调度使用
	 */
	void EnterState(UClass* StateClass);

	/**
	 * 切换到指定类型的状态
	 */
	template<typename T>
	void EnterState()
	{
		EnterState(T::StaticClass());
	}

	/**
	 * 统一状态触发函数，用于拦截并分发玩家输入。等价于原蓝图的 AttackInvoke。
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	void StateInvoke(EInputType InputType);

	/**
	 * 状态重入处理，由当前攻击状态或状态机在收到连击确认时调用，进入下一段攻击。
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	void StateReInvoke(EInputType InputType);

	/**
	 * 连击更新调度，被动画通知 (AN_StateReinvoke) 调用以检查缓存并触发下一段连击。
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	void ComboUpdate();

	/**
	 * 停止状态机并退出当前状态
	 */
	void Stop();

private:
	UPROPERTY()
	UPlayerStateBase* CurrentState;

	UPROPERTY()
	TMap<UClass*, UPlayerStateBase*> StateDic;

public:
	// 提供给外部获取当前状态的接口
	UFUNCTION(BlueprintCallable, Category = "Combat|State")
	UPlayerStateBase* GetCurrentState() const { return CurrentState; }
	
};
