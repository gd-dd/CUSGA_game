#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/StaticMeshComponent.h"
#include "Player/StateMachine/PlayerStateMachine.h"
#include "Player/StateMachine/States/Locomotion/PlayerIdleState.h"
#include "Player/Weapon/WeaponCollider.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	//角色移动组件相关设置
	//设置将旋转朝向运动属性为真
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//调整角色旋转时的速率
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	//调整角色最大行走速度（跟动画匹配）
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	//调整角色最小模拟行走速度
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	//调整角色制动降速行走
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	// 创建并设置摄像机摇臂
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // 默认摇臂长度
	CameraBoom->SetRelativeRotation(FRotator(-30.0f, 0.f, 0.f));	//设置相对旋转
	CameraBoom->bUsePawnControlRotation = true; // 随控制器旋转

	// 创建并设置跟随摄像机
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // 摄像机本身不随控制器旋转

	// 初始化状态机组件
	StateMachine = CreateDefaultSubobject<UPlayerStateMachine>(TEXT("StateMachine"));

	// 初始化武器碰撞组件
	WeaponCollider = CreateDefaultSubobject<UWeaponCollider>(TEXT("WeaponCollider"));

	// 武器静态网格体子对象（默认挂在角色骨骼网格体下）
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("Anbi_Weapon_02Socket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponCollider)
	{
		WeaponCollider->SetWeaponComponent(Weapon);
	}

	if (StateMachine)
	{
		// 初始进入 Idle 状态
		StateMachine->EnterState<UPlayerIdleState>(); 
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::PlayCombatMontage(UAnimMontage* Montage)
{
	if (Montage)
	{
		PlayAnimMontage(Montage);
	}
}
