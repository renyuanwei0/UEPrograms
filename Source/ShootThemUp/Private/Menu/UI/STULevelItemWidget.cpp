// P公司遭遇战,ZTC

#include "Menu/UI/STULevelItemWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTULevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();// 调用父类的初始化函数

    if (LevelSelectButton)
    {
        LevelSelectButton->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnLevelItemClicked); // 添加点击事件处理函数
        LevelSelectButton->OnHovered.AddDynamic(this, &USTULevelItemWidget::OnLevelItemHovered); // 添加鼠标悬浮事件处理函数
        LevelSelectButton->OnUnhovered.AddDynamic(this, &USTULevelItemWidget::OnLevelItemUnhovered);// 添加鼠标离开事件处理函数
    }
}

void USTULevelItemWidget::OnLevelItemClicked()// 关卡选择按钮点击时调用的函数
{
    OnLevelSelected.Broadcast(LevelData);// 广播关卡选择事件
}

void USTULevelItemWidget::SetLevelData(const FLevelData& Data)
{
    LevelData = Data;

    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
    }

    if (LevelImage)// 如果有关卡图像组件
    {
        LevelImage->SetBrushFromTexture(Data.LevelThumb);// 设置关卡缩略图
    }
}

void USTULevelItemWidget::SetSelected(bool IsSelected)// 设置关卡选择状态
{
    if (LevelImage)
    {
        LevelImage->SetColorAndOpacity(IsSelected ? FLinearColor::Blue : FLinearColor::White);// 设置图像颜色为蓝色(选中状态)或白色(未选中状态)
    }
}

void USTULevelItemWidget::OnLevelItemHovered()
{
    if (FrameImage)// 如果有边框图像组件
    {
        FrameImage->SetVisibility(ESlateVisibility::Visible);
    }
}

void USTULevelItemWidget::OnLevelItemUnhovered()
{
    if (FrameImage)
    {
        FrameImage->SetVisibility(ESlateVisibility::Hidden);// 设置为隐藏
    }
}
