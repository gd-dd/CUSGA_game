#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/Game/SaveGame/SaveActGame.h"

void UGameManager::Init()
{
	Super::Init();

	InitializeSaveSlots();

	if (SaveSlotNames.Num() == 0)
	{
		CreateDefaultSaveGame();
		return;
	}

	const FString& PrimarySlotName = SaveSlotNames[0];

	if (UGameplayStatics::DoesSaveGameExist(PrimarySlotName, 0))
	{
		SaveGameRef = Cast<USaveActGame>(UGameplayStatics::LoadGameFromSlot(PrimarySlotName, 0));
	}

	if (SaveGameRef == nullptr)
	{
		// 槽位不存在或加载失败时，回退到默认存档对象。
		CreateDefaultSaveGame();
	}
}

void UGameManager::InitializeSaveSlots()
{
	SaveSlotNames.SetNum(5);

	for (int32 SlotIndex = 0; SlotIndex < SaveSlotNames.Num(); ++SlotIndex)
	{
		SaveSlotNames[SlotIndex] = FString::Printf(TEXT("SaveSlot%d"), SlotIndex);
	}
}

void UGameManager::CreateDefaultSaveGame()
{
	SaveGameRef = Cast<USaveActGame>(UGameplayStatics::CreateSaveGameObject(USaveActGame::StaticClass()));
}
