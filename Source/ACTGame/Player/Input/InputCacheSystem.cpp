#include "InputCacheSystem.h"
#include "Tools/Log/PlayerDebug.h"

UInputCacheSystem::UInputCacheSystem()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 默认允许缓存输入
	bShouldCache = true;
}

void UInputCacheSystem::BeginPlay()
{
	Super::BeginPlay();
}

void UInputCacheSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInputCacheSystem::AddCache(EInputType Input)
{
	// 当允许缓存，或者是第一下攻击（为了起手能入队），才加入队列
	if (bShouldCache || CurrentCacheCount == 0)
	{
		if (Input != EInputType::None)
		{
			DEBUG_PRINT_LOG(2, FColor::Blue, TEXT("[Input] AddCache: Enqueued Input %d"), (int32)Input);
			
			// 如果超出最大缓存长度，移除最旧的一个（出队）
			if (CurrentCacheCount >= MaxCacheLength)
			{
				EInputType DroppedInput;
				if (InputCache.Dequeue(DroppedInput))
				{
					CurrentCacheCount--;
				}
			}
			
			if (InputCache.Enqueue(Input))
			{
				CurrentCacheCount++;
			}
		}
	}
	else
	{
		DEBUG_PRINT_LOG(2, FColor::Orange, TEXT("[Input] AddCache: Blocked (Window Closed)"));
	}
}

bool UInputCacheSystem::GetCache(EInputType& OutInput)
{
	if (CurrentCacheCount > 0 && InputCache.Dequeue(OutInput))
	{
		CurrentCacheCount--;
		DEBUG_PRINT_LOG(2, FColor::Cyan, TEXT("[Input] GetCache: Popped Input %d"), (int32)OutInput);
		return true;
	}
	
	OutInput = EInputType::None;
	return false;
}

void UInputCacheSystem::ClearCache()
{
	InputCache.Empty();
	CurrentCacheCount = 0;
}

void UInputCacheSystem::SetShouldCache(bool bNewShouldCache)
{
	bShouldCache = bNewShouldCache;
}

bool UInputCacheSystem::GetShouldCache() const
{
	return bShouldCache;
}
