// P公司遭遇战,ZTC

#include "AI/Decorators/STUHealthPercentDecorator.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"
//装饰器节点,检查 AI 角色的当前健康百分比是否低于某个阈值
USTUHealthPercentDecorator::USTUHealthPercentDecorator()
{
    NodeName = "Health Percent";
    //节点的名称设置为 "Health Percent"
}

bool USTUHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();//获取行为树组件的 AI 控制器    
    if (!Controller) return false;

    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(Controller->GetPawn());
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    return HealthComponent->GetHealthPercent() <= HealthPercent;//检查角色的当前健康百分比是否小于或等于 HealthPercent 变量的值
}
