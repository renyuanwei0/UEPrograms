// P公司遭遇战,ZTC

#include "AI/Services/STUFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUAIPerceptionComponent.h"
// 构造函数,设置节点名称为"Find Enemy"
USTUFindEnemyService::USTUFindEnemyService()
{
    NodeName = "Find Enemy";
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (const auto Blackboard = OwnerComp.GetBlackboardComponent())// 获取黑板组件
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller);// 获取感知组件
        if (PerceptionComponent)
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());// 将最近的敌人设置到黑板的EnemyActor键上
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);// 调用父类的更新函数
}
