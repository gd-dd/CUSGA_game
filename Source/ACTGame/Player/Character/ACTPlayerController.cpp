#include "ACTPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "EnhancedPlayerInput.h"
#include "InputActionValue.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Input/InputCacheSystem.h"
#include "Player/StateMachine/PlayerStateMachine.h"
#include "Player/StateMachine/States/Combo/Attack/Normal/PlayerAttackState_1.h"
#include "Player/StateMachine/States/Combo/Attack/Special/PlayerSpecialAttackState.h"
#include "Player/StateMachine/States/Locomotion/PlayerIdleState.h"
#include "Player/StateMachine/States/Locomotion/PlayerWalkState.h"

AACTPlayerController::AACTPlayerController()
{
	// 默认初始化逻辑
	InputCacheSystem = CreateDefaultSubobject<UInputCacheSystem>(TEXT("InputCacheSystem"));
	
	PrimaryActorTick.bCanEverTick = true;
	EvadeCooldownTimer = EvadeCooldownDuration; // 初始时允许闪避
}

void AACTPlayerController::BeginPlay()
{
	Super::BeginPlay();
	CurrentPlayerCharacter = Cast<APlayerCharacter>(GetCharacter());
}

void AACTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// 绑定增强输入本地玩家子系统
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (IMC_ACT)
		{
			Subsystem->AddMappingContext(IMC_ACT, 0);
		}
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{

		EnhancedInputComponent->BindAction(IA_NormalAttack, ETriggerEvent::Started, this, &AACTPlayerController::NormalAttack);
		EnhancedInputComponent->BindAction(IA_ACT_Move, ETriggerEvent::Triggered, this, &AACTPlayerController::Move);
		EnhancedInputComponent->BindAction(IA_ACT_Look, ETriggerEvent::Triggered, this, &AACTPlayerController::Look);
		EnhancedInputComponent->BindAction(IA_SpecialAttack, ETriggerEvent::Started, this, &AACTPlayerController::SpecialAttack);
		EnhancedInputComponent->BindAction(IA_Evade, ETriggerEvent::Started, this, &AACTPlayerController::Evade);
		
	}
}

void AACTPlayerController::Move(const FInputActionValue& Value)
{
	// 获取Value里面的2D向量值
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	APlayerCharacter* ControlledCharacter = CurrentPlayerCharacter;

	if (ControlledCharacter != nullptr)
	{
		//三个欧拉角：滚动角Roll（围绕红色X轴旋转），俯仰角Pitch（围绕绿色Y轴旋转），偏航角Yaw（围绕蓝色Z轴旋转）
		//FRotator是三个欧拉角的封装类，成员变量：Pitch、Yaw、Roll
		//获取控制旋转
		const FRotator Rotation = GetControlRotation();
		//角色本地坐标Y轴为正前方，点头方向Roll，摇头方向Yaw，摆头方向Pitch
		//获取Yaw旋转
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//创建一个向量，作为当前角色向前方向向量
		//通过旋转矩阵计算，获取该旋转矩阵的X方向向量，作为向前方向向量
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//创建一个向量，作为当前角色向右方向向量
		//通过旋转矩阵计算，获取该旋转矩阵的Y方向向量，作为向右方向向量
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 将输入与摄像机方向合并，计算出最终的“期望方向向量”
		FVector MoveDirection = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);
		MoveDirection.Z = 0.0f; // 确保在水平面上
		
		// 处理转身逻辑（配合动画根运动和转身动画）
		if (!MoveDirection.IsNearlyZero())
		{
			MoveDirection.Normalize();// 归一化

			// 获取期望方向的绝对旋转
			FRotator TargetRotation = MoveDirection.Rotation();
			
			// 将当前旋转和期望旋转转换为四元数
			//FQuat TargetQuat = TargetRotation.Quaternion();
			//FQuat CurrentInverseQuat = ControlledCharacter->GetActorRotation().Quaternion().Inverse();
			
			// 四元数相乘，得到相对旋转差值 (Delta Rotation)
			//FQuat DeltaQuat = TargetQuat * CurrentInverseQuat;
			
			// 提取出相对的 Yaw 角度差，构造一个相对旋转的旋转体
			//FRotator RelativeRotator(0.0f, DeltaQuat.Rotator().Yaw, 0.0f);
			
			// 将计算出的相对旋转差值直接赋予角色的某个变量（对齐原蓝图逻辑）
			// 直接设置 Actor 的旋转
			//ControlledCharacter->SetActorRotation(RelativeRotator);

			ControlledCharacter->SetTargetRotation(TargetRotation);
		}
	}
}

void AACTPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (this != nullptr)
	{
		// add yaw and pitch input to controller
		AddYawInput(LookAxisVector.X);
		AddPitchInput(LookAxisVector.Y);
	}
}

void AACTPlayerController::NormalAttack()
{
	// 无论当前处于什么状态，先将输入存入缓存
	if (InputCacheSystem)
	{
		InputCacheSystem->AddCache(EInputType::NormalAttack);
	}

	// 然后调用状态机的 StateInvoke 进行事件派发
	// StateMachine 会决定是立即消耗缓存切换状态（如 Idle/Walk 或处于 CanMontageExit 窗口时），
	// 还是等待动画通知（ComboUpdate）来提取缓存。
	if (CurrentPlayerCharacter && CurrentPlayerCharacter->GetStateMachine())
	{
		CurrentPlayerCharacter->GetStateMachine()->StateInvoke(EInputType::NormalAttack);
	}
}

void AACTPlayerController::SpecialAttack()
{
	if (InputCacheSystem)
	{
		InputCacheSystem->AddCache(EInputType::SpecialAttack);
	}

	if (CurrentPlayerCharacter && CurrentPlayerCharacter->GetStateMachine())
	{
		CurrentPlayerCharacter->GetStateMachine()->StateInvoke(EInputType::SpecialAttack);
	}
}

void AACTPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 累加闪避冷却计时器
	if (EvadeCooldownTimer < EvadeCooldownDuration)
	{
		EvadeCooldownTimer += DeltaTime;
	}
}

void AACTPlayerController::Evade()
{
	// 检查冷却时间
	if (EvadeCooldownTimer < EvadeCooldownDuration)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Evade is on cooldown!"));
		return;
	}

	// 记录闪避输入到缓存
	if (InputCacheSystem)
	{
		InputCacheSystem->AddCache(EInputType::Evade);
	}

	// 呼叫状态机处理闪避请求
	if (CurrentPlayerCharacter && CurrentPlayerCharacter->GetStateMachine())
	{
		CurrentPlayerCharacter->GetStateMachine()->StateInvoke(EInputType::Evade);
		// 重置冷却时间
		EvadeCooldownTimer = 0.0f;
	}
}

bool AACTPlayerController::IsInputActionTriggered(const UInputAction* Action) const
{
	if (!Action) return false;

	if (UEnhancedPlayerInput* EnhancedInput = Cast<UEnhancedPlayerInput>(PlayerInput))
	{
		// 通过 GetActionInstance 获取当前 Action 的实例对象
		if (const FInputActionInstance* ActionInstance = EnhancedInput->FindActionInstanceData(Action))
		{
			// 判断触发状态。Triggered 表示持续按住，Started 表示刚刚按下。
			ETriggerEvent TriggerEvent = ActionInstance->GetTriggerEvent();
			return (TriggerEvent == ETriggerEvent::Triggered || TriggerEvent == ETriggerEvent::Started || TriggerEvent == ETriggerEvent::Ongoing);
		}
	}
	
	return false;
}

FInputActionValue AACTPlayerController::GetInputActionValue(const UInputAction* Action) const
{
	if (!Action) return FInputActionValue();
	if (UEnhancedPlayerInput* EnhancedInput = Cast<UEnhancedPlayerInput>(PlayerInput))
	{
		// 直接获取 Action 的当前值，不管它是 bool 还是 Axis1D/2D/3D
		return EnhancedInput->GetActionValue(Action);
	}
	
	return FInputActionValue();
}
