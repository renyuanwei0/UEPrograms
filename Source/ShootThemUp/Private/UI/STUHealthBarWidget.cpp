// P公司遭遇战,ZTC

#include "UI/STUHealthBarWidget.h"
#include "Components/ProgressBar.h"

void USTUHealthBarWidget::SetHealthPercent(float Percent)
{
    if (!HealthProgressBar) return;

    const auto HealthBarVisibility = (Percent > PercentVisibilityThreshold || FMath::IsNearlyZero(Percent))  //
                                         ? ESlateVisibility::Hidden
                                         : ESlateVisibility::Visible;
    HealthProgressBar->SetVisibility(HealthBarVisibility);

    const auto HealthBarColor = Percent > PercentColorThreshold ? GoodColor : BadColor;
    HealthProgressBar->SetFillColorAndOpacity(HealthBarColor);

    HealthProgressBar->SetPercent(Percent);
}
//动态更新游戏角色的血量条显示