// P公司遭遇战,ZTC

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "UI/STUBaseWidget.h"
#include "STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();
    // 绘制准星 (目前被注释掉)
    // DrawCrossHair();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();
    // 创建并添加不同游戏状态下的小组件
    GameWidgets.Add(ESTUMatchState::InProgress, CreateWidget<USTUBaseWidget>(GetWorld(), PlayerHUDWidgetClass));
    GameWidgets.Add(ESTUMatchState::Pause, CreateWidget<USTUBaseWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(ESTUMatchState::GameOver, CreateWidget<USTUBaseWidget>(GetWorld(), GameOverWidgetClass));
    // 将创建的小组件添加到视口并隐藏
    for (auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if (!GameWidget) continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    // 监听游戏状态的变化事件
    if (GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged);
        }
    }
}

void ASTUGameHUD::OnMatchStateChanged(ESTUMatchState State)// 游戏状态变化时调用
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    // 根据当前游戏状态显示对应的小组件
    if (GameWidgets.Contains(State))
    {
        CurrentWidget = GameWidgets[State];
    }

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
        CurrentWidget->Show();
    }

    UE_LOG(LogSTUGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}

void ASTUGameHUD::DrawCrossHair()
{
    // 获取画布中心坐标
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);
    // 准星线条参数
    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;
    // 绘制水平和垂直的准星线条
    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
