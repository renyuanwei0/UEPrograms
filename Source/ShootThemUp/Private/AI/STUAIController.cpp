// P公司遭遇战,ZTC

#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"
#include "Components/STURespawnComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASTUAIController::ASTUAIController()
{
    STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUPerceptionComponent");// 创建感知组件
    SetPerceptionComponent(*STUAIPerceptionComponent);

    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");// 创建重生组件

    bWantsPlayerState = true;// 需要玩家状态
}

void ASTUAIController::OnPossess(APawn* InPawn)// 控制器获得对应的Pawn时
{
    Super::OnPossess(InPawn);

    if (const auto STUCharacter = Cast<ASTUAICharacter>(InPawn))// 如果获取的Pawn是ASTUAICharacter类型
    {
        RunBehaviorTree(STUCharacter->BehaviorTreeAsset);// 运行该角色的行为树
    }
}

void ASTUAIController::Tick(float DeltaTime)// 每帧更新
{
    Super::Tick(DeltaTime);
    SetFocus(GetFocusOnActor());
}

AActor* ASTUAIController::GetFocusOnActor() const// 获取当前关注的Actor
{
    if (!GetBlackboardComponent()) return nullptr; // 检查黑板组件是否存在
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));// 从黑板组件中获取关注的目标Actor
}
