#pragma once

#include "CoreMinimal.h"
#include "Player/Base/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPlayerStateMachine;
class UAnimMontage;
class UActionDataContainer;
class UWeaponCollider;
class UStaticMeshComponent;

/**
 * 玩家实际操控的角色实体类 (PlayerCharacter)
 * 职责: 1. 管理相机(SpringArm, Camera)组件。
 *       2. 挂载并初始化状态机组件(StateMachine)。
 *       3. 提供播放战斗动画蒙太奇的接口供状态机调用。
 */
UCLASS()
class ACTGAME_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// 供状态机调用的动画播放接口
	UFUNCTION(BlueprintCallable, Category = "Combat|Animation")
	void PlayCombatMontage(UAnimMontage* Montage);

	// 获取/设置目标旋转 (用于平滑转身和移动)
	UFUNCTION(BlueprintCallable, Category = "Combat|Movement")
	FRotator GetTargetRotation() const { return TargetRotation; }

	UFUNCTION(BlueprintCallable, Category = "Combat|Movement")
	void SetTargetRotation(const FRotator& InRotation) { TargetRotation = InRotation; }

	// 获取状态机组件
	UPlayerStateMachine* GetStateMachine() const { return StateMachine; }

	// 获取角色绑定的动作数据容器
	UActionDataContainer* GetActionDataContainer() const { return ActionDataContainer; }

protected:
	// 摄像机摇臂
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// 跟随摄像机
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// 玩家状态机
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State Machine", meta = (AllowPrivateAccess = "true"))
	UPlayerStateMachine* StateMachine;

	// 缓存的绝对目标旋转，用于移动和转身插值
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FRotator TargetRotation;

	// 角色绑定的战斗动作数据容器（包含所有攻击、闪避的动画与属性配置）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Data")
	UActionDataContainer* ActionDataContainer;

	// 武器碰撞组件（命中检测与事件分发）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Collision", meta = (AllowPrivateAccess = "true"))
	UWeaponCollider* WeaponCollider;

	// 武器静态网格体组件（挂在角色骨骼网格体下，蓝图中可直接设置网格资源）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Weapon;
};
