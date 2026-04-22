// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ActionData.generated.h"

/**
 * 战斗动作类型枚举
 */
UENUM(BlueprintType)
enum class EActionType : uint8
{
	NormalAttack	UMETA(DisplayName = "普通攻击"),
	SpecialAttack	UMETA(DisplayName = "特殊攻击"),
	Evade			UMETA(DisplayName = "闪避"),
	Block			UMETA(DisplayName = "格挡")
};

/**
 * 独立战斗动作的数据载体
 */
UCLASS(BlueprintType, Blueprintable)
class ACTGAME_API UActionData : public UDataAsset
{
	GENERATED_BODY()

public:
	// 动作类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Info")
	EActionType ActionType = EActionType::NormalAttack;

	// 当前动作需要播放的主要蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Info")
	UAnimMontage* ActionMontage = nullptr;

	// 额外/备用蒙太奇（如闪避的前后左右区分，或攻击的分支变化）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Info")
	TArray<UAnimMontage*> ExtraMontages;

	// 连击下一段状态（数据驱动派生）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Info")
	TSubclassOf<class UPlayerStateBase> NextComboState;

	// 未来可以在此扩展更多属性，例如：
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Info")
	// float DamageMultiplier = 1.0f;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Info")
	// float EnergyCost = 0.0f;
};
