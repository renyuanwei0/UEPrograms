// P公司遭遇战,ZTC

#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/STUHealthBarWidget.h"
#include "Components/STUHealthComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = ASTUAIController::StaticClass();
    // 禁用角色自身的旋转
    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())// 启用控制器旋转,设置旋转速率
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }

    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
    HealthWidgetComponent->SetupAttachment(GetRootComponent());
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void ASTUAICharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthWidgetComponent);// 检查血量条组件是否正确创建
}

void ASTUAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateHealthWidgetVisibility();// 更新血量条组件的可见性
}

void ASTUAICharacter::OnDeath()
{
    Super::OnDeath();
    // 获取AI控制器并清理大脑组件
    const auto STUController = Cast<AAIController>(Controller);
    if (STUController && STUController->BrainComponent)
    {
        STUController->BrainComponent->Cleanup();
    }
}
// 角色生命值改变时
void ASTUAICharacter::OnHealthChanged(float Health, float HealthDelta)
{
    Super::OnHealthChanged(Health, HealthDelta);
    // 获取血量条小组件并更新血量百分比
    const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
    if (!HealthBarWidget) return;
    HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}
// 更新血量条组件的可见性
void ASTUAICharacter::UpdateHealthWidgetVisibility()
{
    if (!GetWorld() ||                              //// 检查世界和玩家控制器是否存在
        !GetWorld()->GetFirstPlayerController() ||  //
        !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator())
        return;
    // 计算玩家和AI角色之间的距离
    const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
    const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
    HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);// 根据距离设置血量条组件的可见性
}
