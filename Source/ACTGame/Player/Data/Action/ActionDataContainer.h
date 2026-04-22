// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ActionData.h"
#include "ActionDataContainer.generated.h"

class UPlayerStateBase;

/**
 * 动作数据容器映射表
 * 负责建立具体状态类到单招数据(ActionData)的映射关系
 */
UCLASS(BlueprintType, Blueprintable)
class ACTGAME_API UActionDataContainer : public UDataAsset
{
	GENERATED_BODY()

public:
	// 核心：状态类 -> 独立动作数据资产 的映射表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action Container")
	TMap<TSubclassOf<UPlayerStateBase>, UActionData*> StateToDataMap;
};
