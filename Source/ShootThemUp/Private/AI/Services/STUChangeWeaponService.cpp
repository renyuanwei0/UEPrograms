// P公司遭遇战,ZTC

#include "AI/Services/STUChangeWeaponService.h"
#include "Components/STUWeaponComponent.h"
#include "AIController.h"
#include "STUUtils.h"

// 构造函数,设置节点名称为"Change Weapon"
USTUChangeWeaponService::USTUChangeWeaponService()
{
    NodeName = "Change Weapon";
}

// 节点更新函数
void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 获取AI控制器
    if (const auto Controller = OwnerComp.GetAIOwner())
    {
        // 获取武器组件
        const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
        // 如果武器组件存在,且切换武器的概率大于0,且随机数小于等于概率
        if (WeaponComponent && Probability > 0 && FMath::FRand() <= Probability)
        {
            // 切换到下一个武器
            WeaponComponent->NextWeapon();
        }
    }

    // 调用父类的更新函数
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
