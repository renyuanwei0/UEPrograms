// P公司遭遇战,ZTC

#include "UI/STUGameOverWidget.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"
#include "UI/STUPlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "STUUtils.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USTUGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetWorld())
    {
        if (const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged);
        }
    }

    if (ResetLevelButton)
    {
        ResetLevelButton->OnClicked.AddDynamic(this, &USTUGameOverWidget::OnResetLevel);
    }
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State)// 当匹配状态改变时调用此函数
{
    if (State == ESTUMatchState::GameOver)// 如果状态为"游戏结束",则更新玩家统计数据
    {
        UpdatePlayersStat();
    }
}

void USTUGameOverWidget::UpdatePlayersStat()
{
    if (!GetWorld() || !PlayerStatBox) return;

    PlayerStatBox->ClearChildren();// 清空PlayerStatBox中的子控件

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;
        // 创建并配置玩家统计行小部件
        const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
        if (!PlayerStatRowWidget) continue;

        PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStatRowWidget->SetKills(STUUtils::TextFromInt(PlayerState->GetKillsNum()));
        PlayerStatRowWidget->SetDeaths(STUUtils::TextFromInt(PlayerState->GetDeathsNum()));
        PlayerStatRowWidget->SetTeam(STUUtils::TextFromInt(PlayerState->GetTeamID()));
        PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());
        PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor());
        // 将玩家统计行小部件添加到PlayerStatBox
        PlayerStatBox->AddChild(PlayerStatRowWidget);   
    }
}

void USTUGameOverWidget::OnResetLevel()
{
    // 获取当前关卡名称
    const auto CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));// 使用GameplayStatics打开当前关卡
}
