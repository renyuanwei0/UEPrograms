// P公司遭遇战,ZTC

#include "Menu/STUMenuGameModeBase.h"
#include "Menu/STUMenuPlayerController.h"
#include "Menu/UI/STUMenuHUD.h"

ASTUMenuGameModeBase::ASTUMenuGameModeBase()
{
    PlayerControllerClass = ASTUMenuPlayerController::StaticClass();
    HUDClass = ASTUMenuHUD::StaticClass();
}
