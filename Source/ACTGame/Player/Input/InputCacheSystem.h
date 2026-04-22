#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Queue.h"
#include "InputCacheSystem.generated.h"

// 声明输入类型枚举
UENUM(BlueprintType)
enum class EInputType : uint8
{
	None    UMETA(DisplayName = "None"),
	NormalAttack  UMETA(DisplayName = "NormalAttack"),
	SpecialAttack UMETA(DisplayName = "SpecialAttack"),
	Evade   UMETA(DisplayName = "Evade")
};

/**
 * 动作游戏核心输入缓存组件 (Input Cache System)
 * 职责: 实现硬核动作游戏的“连招预输入”机制。
 * 说明: 
 * 1. 它维护一个指令队列（FIFO），当玩家按下按键时，不立刻执行，而是放入队列。
 * 2. 结合动画通知(ANS_InputCache)，在特定帧区间内开启 `bShouldCache`，允许玩家输入下一段攻击。
 * 3. 状态机在执行连段判定时，会来这里“消费(Consume)”指令。
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTGAME_API UInputCacheSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInputCacheSystem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 添加输入到缓存队列
	UFUNCTION(BlueprintCallable, Category = "Input Cache")
	void AddCache(EInputType Input);

	// 获取并移除队列首个输入
	UFUNCTION(BlueprintCallable, Category = "Input Cache")
	bool GetCache(EInputType& OutInput);

	// 清空缓存队列
	UFUNCTION(BlueprintCallable, Category = "Input Cache")
	void ClearCache();

	// 设置是否允许缓存输入
	UFUNCTION(BlueprintCallable, Category = "Input Cache")
	void SetShouldCache(bool bNewShouldCache);

	// 获取当前是否允许缓存输入
	UFUNCTION(BlueprintPure, Category = "Input Cache")
	bool GetShouldCache() const;

private:
	// 存储输入的队列（使用更高效的 TQueue 代替 TArray）
	TQueue<EInputType> InputCache;
	
	// 当前缓存长度计数（TQueue不自带元素计数）
	int32 CurrentCacheCount = 0;

	// 最大缓存长度
	UPROPERTY(EditAnywhere, Category = "Input Cache")
	int32 MaxCacheLength = 10;

	// 控制是否接受新的输入缓存
	bool bShouldCache;
};
