// P公司遭遇战,ZTC

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

ASTUProjectile::ASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);// 设置碰撞为查询模式
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);// 设置所有通道的碰撞响应为阻挡
    CollisionComponent->bReturnMaterialOnMove = true;// 设置移动时返回材质
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");// 创建投射物移动组件
    MovementComponent->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;

    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");// 创建武器特效组件
}

void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();
    // 检查移动组件、碰撞组件和特效组件是否存在
    check(MovementComponent);
    check(CollisionComponent);
    check(WeaponFXComponent);

    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;// 设置初始速度方向和大小
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);// 忽略发射者的碰撞
    CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);// 添加碰撞事件回调函数
    SetLifeSpan(LifeSeconds);// 设置生存时间
}

void ASTUProjectile::OnProjectileHit(// 投射物碰撞事件的回调函数
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!GetWorld()) return;
    // 立即停止移动
    MovementComponent->StopMovementImmediately();

    // 造成伤害
    UGameplayStatics::ApplyRadialDamage(GetWorld(),  //
        DamageAmount,                                //
        GetActorLocation(),                          //
        DamageRadius,                                //
        UDamageType::StaticClass(),                  //
        {GetOwner()},                                //
        this,                                        //
        GetController(),                             //
        DoFullDamage);

    // DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);
    WeaponFXComponent->PlayImpactFX(Hit);// 播放撞击特效
    Destroy();
}

AController* ASTUProjectile::GetController() const// 获取投射物的控制器
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}
