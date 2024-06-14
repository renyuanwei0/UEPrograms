// P公司遭遇战,ZTC

#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const// 获取最近的敌人
{
    TArray<AActor*> PercieveActors;// 获取所有感知到的角色
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
    if (PercieveActors.Num() == 0)// 如果没有通过视觉感知到任何角色,则检查伤害感知
    {
        GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PercieveActors);
        if (PercieveActors.Num() == 0) return nullptr;
    }

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    float BestDistance = MAX_FLT;// 找到最近的敌人
    AActor* BestPawn = nullptr;
    for (const auto PercieveActor : PercieveActors)
    {
        const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PercieveActor);

        const auto PercievePawn = Cast<APawn>(PercieveActor);
        const auto AreEnemies = PercievePawn && STUUtils::AreEnemies(Controller, PercievePawn->Controller);// 判断是否为敌人
        // 如果敌人存在且未死亡,则计算距离并更新最近的敌人
        if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
        {
            const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PercieveActor;
            }
        }
    }

    return BestPawn;
}
