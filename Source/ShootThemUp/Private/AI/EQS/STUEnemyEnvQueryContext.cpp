// P公司遭遇战,ZTC

#include "AI/EQS/STUEnemyEnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void USTUEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const//当前正在运行的环境查询实例。及用于存储查询结果的上下文数据。
{
    const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());//从查询实例中获取查询所有者,并将其转换为 AActor 类型。
    
    const auto Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
    if (!Blackboard) return;
    
    const auto ContextActor = Blackboard->GetValueAsObject(EnemyActorKeyName);//从黑板组件中获取名为 EnemyActorKeyName 的对象值,并将其存储在 ContextActor 变量中。
    UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(ContextActor));
    //使用 UEnvQueryItemType_Actor::SetContextHelper 函数,将获取到的 ContextActor 设置为查询上下文数据的一部分。
    //这样做是为了在后续的环境查询中使用这个上下文数据。
}
