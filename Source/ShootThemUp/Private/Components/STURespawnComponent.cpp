// P公司遭遇战,ZTC

#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"

USTURespawnComponent::USTURespawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;// 设置组件不需要每帧更新
}

void USTURespawnComponent::Respawn(int32 RespawnTime)// Respawn 函数,用于启动重生倒计时
{
    if (!GetWorld()) return;

    RespawnCountDown = RespawnTime;// 设置重生倒计时时间
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimerUpdate, 1.0f, true);
    // 使用 GetWorld()->GetTimerManager() 启动一个每秒钟更新一次的定时器,调用 RespawnTimerUpdate() 函数
}

void USTURespawnComponent::RespawnTimerUpdate()
{
    if (--RespawnCountDown == 0)// 递减重生倒计时
    {
        if (!GetWorld()) return;
        GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);// 停止定时器
        // 获取 ASTUGameModeBase 类型的游戏模式
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (!GameMode) return;
        // 调用 ASTUGameModeBase 类的 RespawnRequest 函数,请求玩家重生
        GameMode->RespawnRequest(Cast<AController>(GetOwner()));
    }
}
// IsRespawnInProgress 函数,用于检查是否正在进行重生倒计时
bool USTURespawnComponent::IsRespawnInProgress() const
{
    // 检查当前世界是否有效,并检查 RespawnTimerHandle 定时器是否正在运行
    return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}
