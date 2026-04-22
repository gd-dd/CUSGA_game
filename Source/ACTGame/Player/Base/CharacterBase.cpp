#include "CharacterBase.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MaxHealth = 100.0f;
	Health = MaxHealth;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterBase::ReceiveHit(float DamageAmount)
{
	Health -= DamageAmount;
	
	if (Health <= 0.0f)
	{
		Health = 0.0f;
		// 此处可以添加死亡逻辑
	}
}
