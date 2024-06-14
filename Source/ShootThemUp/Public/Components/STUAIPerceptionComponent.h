// P公司遭遇战,ZTC

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "STUAIPerceptionComponent.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUAIPerceptionComponent : public UAIPerceptionComponent
{
    GENERATED_BODY()

public:
    AActor* GetClosestEnemy() const;
};
