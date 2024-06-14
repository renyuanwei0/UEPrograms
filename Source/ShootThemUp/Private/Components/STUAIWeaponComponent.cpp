// P公司遭遇战,ZTC

#include "Components/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"

void USTUAIWeaponComponent::StartFire()
{
    if (!CanFire()) return;// 检查是否可以开火

    if (CurrentWeapon->IsAmmoEmpty())// 如果当前武器没有弹药,切换到下一个武器
    {
        NextWeapon();
    }
    else// 否则开始开火
    {
        CurrentWeapon->StartFire();
    }
}

void USTUAIWeaponComponent::NextWeapon()// 切换到下一个武器
{
    if (!CanEquip()) return;// 检查是否可以切换武器
    // 找到下一个可用的武器
    int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    while (NextIndex != CurrentWeaponIndex)
    {
        if (!Weapons[NextIndex]->IsAmmoEmpty()) break;
        NextIndex = (NextIndex + 1) % Weapons.Num();
    }
    // 如果找到了下一个可用的武器,则切换到该武器
    if (CurrentWeaponIndex != NextIndex)
    {
        CurrentWeaponIndex = NextIndex;
        EquipWeapon(CurrentWeaponIndex);
    }
}
