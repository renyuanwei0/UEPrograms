// P公司遭遇战,ZTC

#include "Dev/STUDevDamageActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ASTUDevDamageActor::ASTUDevDamageActor()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");// 创建一个默认的 SceneComponent 组件作为根组件
    SetRootComponent(SceneComponent);
}

void ASTUDevDamageActor::BeginPlay()
{
    Super::BeginPlay();
}

void ASTUDevDamageActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);// 在当前位置绘制一个半径为 Radius 的调试球体,颜色为 SphereColor
    UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, DoFullDamage);
    // 使用 UGameplayStatics::ApplyRadialDamage 函数对当前位置周围的目标施加伤害
}
