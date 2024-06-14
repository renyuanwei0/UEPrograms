// P公司遭遇战,ZTC

#include "AI/EQS/EnvQueryTest_PickupCouldBeTaken.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Pickups/STUBasePickup.h"

UEnvQueryTest_PickupCouldBeTaken::UEnvQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer)  //
    : Super(ObjectInitializer)
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
    // Cost = EEnvTestCost::Low: 设置测试的开销级别为低。
    // ValidItemType = UEnvQueryItemType_ActorBase::StaticClass(): 设置有效的项目类型为 UEnvQueryItemType_ActorBase。
    // SetWorkOnFloatValues(false): 设置该测试不处理浮点值。
}

void UEnvQueryTest_PickupCouldBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const//当前正在运行的环境查询实例
{
    // 获取查询实例的所有者 DataOwner。
    // 绑定 BoolValue 数据,用于获取是否希望项目可被拾取的布尔值。
    // 使用 FEnvQueryInstance::ItemIterator 遍历查询结果中的每个项目:
    // 获取当前项目对应的角色 ItemActor。
    // 尝试将 ItemActor 转换为 ASTUBasePickup 类型。如果转换失败,则跳过当前项目。
    // 调用 ASTUBasePickup::CouldBeTaken 函数检查该拾取品是否可被拾取。
    // 使用 It.SetScore 函数设置当前项目的得分,基于 CouldBeTaken 值和 WantsBeTakable 值。
    const auto DataOwner = QueryInstance.Owner.Get();
    BoolValue.BindData(DataOwner, QueryInstance.QueryID);
    const bool WantsBeTakable = BoolValue.GetValue();

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        const auto ItemActor = GetItemActor(QueryInstance, It.GetIndex());
        const auto PickupActor = Cast<ASTUBasePickup>(ItemActor);
        if (!PickupActor) continue;

        const auto CouldBeTaken = PickupActor->CouldBeTaken();
        It.SetScore(TestPurpose, FilterType, CouldBeTaken, WantsBeTakable);
    }
}
