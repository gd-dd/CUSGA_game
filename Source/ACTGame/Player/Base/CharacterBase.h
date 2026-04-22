#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

/**
 * 所有战斗实体（包括玩家和敌人）的基础外壳类。
 * 职责: 处理角色最底层的通用属性和行为，例如：物理碰撞、基础生命值、受击判定接口。
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

	// 处理受击逻辑
	//UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void ReceiveHit(float DamageAmount);

protected:
	// 生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health;
	
	// 最大生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth;
};
