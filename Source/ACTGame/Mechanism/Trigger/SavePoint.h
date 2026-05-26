#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SavePoint.generated.h"

UCLASS()
class ACTGAME_API ASavePoint : public ATriggerBox
{
	GENERATED_BODY()

public:
	ASavePoint();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
