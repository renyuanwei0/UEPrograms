// P公司遭遇战,ZTC

#include "Menu/STUMenuPlayerController.h"

void ASTUMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
}
