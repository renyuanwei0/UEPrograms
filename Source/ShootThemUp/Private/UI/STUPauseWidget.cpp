// P公司遭遇战,ZTC

#include "UI/STUPauseWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"

void USTUPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
    }
}

void USTUPauseWidget::OnClearPause()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
}
//提供一个按钮,用于清除游戏的暂停状态。