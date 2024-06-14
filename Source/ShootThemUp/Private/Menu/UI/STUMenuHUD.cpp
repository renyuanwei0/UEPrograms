// P公司遭遇战,ZTC

#include "Menu/UI/STUMenuHUD.h"
#include "UI/STUBaseWidget.h"

void ASTUMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    if (const auto MenuWidget = CreateWidget<USTUBaseWidget>(GetWorld(), MenuWidgetClass))
    {
        MenuWidget->AddToViewport();
        MenuWidget->Show();
    }
}
