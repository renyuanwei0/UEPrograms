// P公司遭遇战,ZTC

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUBaseWidget.generated.h"

class USoundCue;

UCLASS()
class SHOOTTHEMUP_API USTUBaseWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void Show();

protected:
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* ShowAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* OpenSound;
};
