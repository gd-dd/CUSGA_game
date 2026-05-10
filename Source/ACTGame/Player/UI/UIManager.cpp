#include "UIManager.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Player/Character/PlayerCharacter.h"
#include "PlayerHud.h"

AUIManager::AUIManager()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UPlayerHud> PlayerHudBlueprintClass(TEXT("/Game/Blueprint/UI/WBP_PlayerHud"));
	if (PlayerHudBlueprintClass.Succeeded())
	{
		PlayerHudClass = PlayerHudBlueprintClass.Class;
	}
}

void AUIManager::BeginPlay()
{
	Super::BeginPlay();

	InitializeUI();
}

void AUIManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController || !PlayerController->IsLocalController())
	{
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter)
	{
		DestroyUI();
		return;
	}

	if (!PlayerHudInstance)
	{
		InitializeUI();
	}
}

void AUIManager::InitializeUI()
{
	if (PlayerHudInstance)
	{
		return;
	}

	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController || !PlayerController->IsLocalController() || !PlayerHudClass)
	{
		return;
	}

	PlayerHudInstance = CreateWidget<UPlayerHud>(PlayerController, PlayerHudClass);
	if (!PlayerHudInstance)
	{
		return;
	}

	PlayerHudInstance->AddToViewport();

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn()))
	{
		PlayerHudInstance->InitializePlayerHud(PlayerCharacter);
	}
}

void AUIManager::DestroyUI()
{
	if (!PlayerHudInstance)
	{
		return;
	}

	PlayerHudInstance->RemoveFromParent();
	PlayerHudInstance = nullptr;
}
