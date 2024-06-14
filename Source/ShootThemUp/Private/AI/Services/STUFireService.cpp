// P公司遭遇战,ZTC

#include "AI/Services/STUFireService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

USTUFireService::USTUFireService()
{
    NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);// 检查是否有瞄准的目标

    if (const auto Controller = OwnerComp.GetAIOwner())
    {
        if (const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn())) // 获取武器组件
        {
            HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();// 如果有瞄准的目标,则开始开火;否则停止开火
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);// 调用父类的更新函数
}
