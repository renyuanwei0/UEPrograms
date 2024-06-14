// P公司遭遇战,ZTC

#include "UI/STUGameDataWidget.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"

int32 USTUGameDataWidget::GetCurrentRoundNum() const// 获取当前回合数
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetCurrentRoundNum() : 0;// 如果游戏模式不为空,则返回当前回合数,否则返回0
}

int32 USTUGameDataWidget::GetTotalRoundsNum() const// 获取总回合数
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetGameData().RoundsNum : 0;
}

int32 USTUGameDataWidget::GetRoundSecondsRemaining() const// 获取当前回合剩余时间
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetRoundSecondsRemaining() : 0;
}

ASTUGameModeBase* USTUGameDataWidget::GetSTUGameMode() const// 获取游戏模式
{
    return GetWorld() ? Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;// 获取当前世界对象,并将其转换为游戏模式
}

ASTUPlayerState* USTUGameDataWidget::GetSTUPlayerState() const
{
    return GetOwningPlayer() ? Cast<ASTUPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;// 获取拥有该小组件的玩家,并将其转换为玩家状态
}
