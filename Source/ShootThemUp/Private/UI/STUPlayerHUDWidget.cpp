// P公司遭遇战,ZTC

#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"
#include "Components/ProgressBar.h"
#include "Player/STUPlayerState.h"

void USTUPlayerHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);// 添加新角色进入的通知回调
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void USTUPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(NewPawn);
    if (HealthComponent && !HealthComponent->OnHealthChanged.IsBoundToObject(this))// 如果健康组件存在且没有绑定健康变化事件
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
    }
    UpdateHealthBar();    // 更新血量条

}
// 角色血量变化时调用的函数
void USTUPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
    if (HealthDelta < 0.0f)
    {
        OnTakeDamage();

        if (!IsAnimationPlaying(DamageAnimation))
        {
            PlayAnimation(DamageAnimation);
        }
    }
    UpdateHealthBar();
}
// 获取当前血量百分比的函数
float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent(); // 返回当前血量百分比
}
// 获取当前武器UI数据的函数
bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->GetCurrentWeaponUIData(UIData);
}
// 获取当前武器弹药数据的函数
bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}
// 判断玩家是否存活的函数
bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}
// 判断玩家是否正在观战的函数
bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;    // 返回玩家是否正在观战的结果
}
// 获取玩家击杀数的函数
int32 USTUPlayerHUDWidget::GetKillsNum() const
{
    const auto Controller = GetOwningPlayer();
    if (!Controller) return 0;
    // 获取玩家状态,并返回击杀数
    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    return PlayerState ? PlayerState->GetKillsNum() : 0;
}
// 更新血量条的函数
void USTUPlayerHUDWidget::UpdateHealthBar()
{
    if (HealthProgressBar)
    {
        HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor); // 根据当前血量百分比设置血量条的颜色
    }
}

FString USTUPlayerHUDWidget::FormatBullets(int32 BulletsNum) const// 格式化默认弹药数量的函数
{
    const int32 MaxLen = 3;
    const TCHAR PrefixSymbol = '0';// 前缀填充字符

    auto BulletStr = FString::FromInt(BulletsNum);// 将弹药数转换为字符串
    const auto SymbolsNumToAdd = MaxLen - BulletStr.Len();// 计算需要添加的前缀字符数

    if (SymbolsNumToAdd > 0)
    {
        BulletStr = FString::ChrN(SymbolsNumToAdd, PrefixSymbol).Append(BulletStr);
    }
    // 返回格式化后的弹药数字符串
    return BulletStr;
}
