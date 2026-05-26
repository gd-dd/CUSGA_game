#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

/**
 * 所有战斗实体（包括玩家和敌人）的基础外壳类。
 * 职责: 处理角色最底层的通用属性和行为，例如：物理碰撞与受击接口约定。
 * 说明: 抽象类（Abstract），不能直接实例化，必须被继承使用。
 */
UCLASS(Abstract)
class ACTGAME_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 受击入口由具体子类实现，基类只保留统一接口约定。
	virtual void ReceiveHit(float DamageAmount);
};
