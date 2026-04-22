#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"

// 声明项目专属的战斗系统日志分类
DECLARE_LOG_CATEGORY_EXTERN(LogACTGameCombat, Log, All);

// 全局屏幕打印开关 (设为 0 可一键关闭所有屏幕调试输出，但后台日志仍保留)
#define ENABLE_SCREEN_DEBUG 1

#if !UE_BUILD_SHIPPING

/**
 * @brief 同时输出到屏幕和后台日志 (Output Log)
 * @param Key 屏幕打印的Key (-1表示不覆盖，>0表示覆盖同一Key的旧信息)
 * @param Color 屏幕打印的颜色 (如 FColor::Red)
 * @param Format 格式化字符串 (如 TEXT("Hit: %f"))
 * @param ... 格式化参数
 */
#define DEBUG_PRINT_LOG(Key, Color, Format, ...) \
{ \
	FString Msg = FString::Printf(Format, ##__VA_ARGS__); \
	UE_LOG(LogACTGameCombat, Log, TEXT("%s"), *Msg); \
	if (ENABLE_SCREEN_DEBUG && GEngine) \
	{ \
		GEngine->AddOnScreenDebugMessage(Key, 2.0f, Color, Msg); \
	} \
}

/**
 * @brief 仅输出到后台日志，不显示在屏幕上 (适合高频日志)
 */
#define DEBUG_LOG(Format, ...) \
{ \
	UE_LOG(LogACTGameCombat, Log, Format, ##__VA_ARGS__); \
}

#else

// 在 Shipping 打包模式下，所有宏自动替换为空，实现零性能损耗
#define DEBUG_PRINT_LOG(Key, Color, Format, ...)
#define DEBUG_LOG(Format, ...)

#endif
