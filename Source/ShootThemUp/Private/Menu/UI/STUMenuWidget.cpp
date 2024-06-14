// P公司遭遇战,ZTC

#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Menu/UI/STULevelItemWidget.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All);
// 当小部件初始化时调用的函数
void USTUMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartGameButton)// 为开始游戏按钮添加点击事件处理程序
    {
        StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
    }

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
    }

    InitLevelItems(); // 初始化关卡列表项
}

void USTUMenuWidget::InitLevelItems()
{
    const auto STUGameInstance = GetSTUGameInstance();// 获取 STUGameInstance 实例
    if (!STUGameInstance) return;

    checkf(STUGameInstance->GetLevelsData().Num() != 0, TEXT("Levels data must not be empty!"));// 确保关卡数据不为空

    if (!LevelItemsBox) return;
    LevelItemsBox->ClearChildren();// 清空关卡列表项容器

    for (auto LevelData : STUGameInstance->GetLevelsData()) // 遍历关卡数据,创建并添加关卡列表项
    {
        const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
        if (!LevelItemWidget) continue;

        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected);

        LevelItemsBox->AddChild(LevelItemWidget);
        LevelItemWidgets.Add(LevelItemWidget);
    }

    if (STUGameInstance->GetStartupLevelData().LevelName.IsNone())// 选择默认关卡
    {
        OnLevelSelected(STUGameInstance->GetLevelsData()[0]);
    }
    else
    {
        OnLevelSelected(STUGameInstance->GetStartupLevelData());
    }
}

void USTUMenuWidget::OnLevelSelected(const FLevelData& Data)// 当选择关卡时调用的函数
{
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance) return;

    STUGameInstance->SetStartupLevelData(Data);// 更新启动关卡数据

    for (auto LevelItemWidget : LevelItemWidgets)// 更新关卡列表项的选择状态
    {
        if (LevelItemWidget)
        {
            const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
            LevelItemWidget->SetSelected(IsSelected);
        }
    }
}

void USTUMenuWidget::OnStartGame()
{
    PlayAnimation(HideAnimation);// 播放隐藏动画
    UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound); // 播放开始游戏音效
}

void USTUMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)// 当动画播放完成时调用的函数
{
    if (Animation != HideAnimation) return;// 如果不是隐藏动画,则返回

    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance) return;

    UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevelData().LevelName);// 打开启动关卡
}

void USTUMenuWidget::OnQuitGame()// 当退出游戏按钮被点击时调用的函数
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

USTUGameInstance* USTUMenuWidget::GetSTUGameInstance() const// 获取 STUGameInstance 实例
{
    if (!GetWorld()) return nullptr;
    return GetWorld()->GetGameInstance<USTUGameInstance>();
}
