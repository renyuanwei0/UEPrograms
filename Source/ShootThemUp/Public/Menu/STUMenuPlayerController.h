// P公司遭遇战,ZTC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUMenuPlayerController.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUMenuPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
};
