#include "SavePoint.h"

#include "Components/ShapeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Character/PlayerCharacter.h"
#include "Player/Health/PlayerHealth.h"
#include "Tools/Game/GameManager.h"
#include "Tools/Game/PlayerData.h"
#include "Tools/Game/SaveGame/SaveActGame.h"

ASavePoint::ASavePoint()
{
	if (UShapeComponent* ShapeComponent = GetCollisionComponent())
	{
		ShapeComponent->SetGenerateOverlapEvents(true);
		ShapeComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ShapeComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		ShapeComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

void ASavePoint::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter == nullptr)
	{
		return;
	}

	UGameManager* GameManager = GetGameInstance<UGameManager>();
	if (GameManager == nullptr || GameManager->bIsSaving || GameManager->SaveGameRef == nullptr)
	{
		return;
	}

	UPlayerHealth* PlayerHealth = PlayerCharacter->GetPlayerHealth();
	if (PlayerHealth == nullptr)
	{
		return;
	}

	FPlayerData PlayerData;
	PlayerData.HP = PlayerHealth->GetCurrentHealth();
	PlayerData.Transform = PlayerCharacter->GetActorTransform();
	PlayerData.CameraRotation = PlayerCharacter->GetControlRotation();
	PlayerData.bHasSavedData = true;

	GameManager->SaveGameRef->PlayerData = PlayerData;
	GameManager->bIsSaving = true;

	const FString SlotName = GameManager->SaveSlotNames.IsValidIndex(0) ? GameManager->SaveSlotNames[0] : TEXT("SaveSlot0");
	TWeakObjectPtr<UGameManager> WeakGameManager(GameManager);

	FAsyncSaveGameToSlotDelegate SaveFinishedDelegate;
	SaveFinishedDelegate.BindLambda([WeakGameManager](const FString& SavedSlotName, const int32 UserIndex, bool bSuccess)
	{
		if (WeakGameManager.IsValid())
		{
			WeakGameManager->bIsSaving = false;
		}
	});

	UGameplayStatics::AsyncSaveGameToSlot(GameManager->SaveGameRef, SlotName, 0, SaveFinishedDelegate);
}
