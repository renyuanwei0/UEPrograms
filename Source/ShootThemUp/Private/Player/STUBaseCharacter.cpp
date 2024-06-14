// P公司遭遇战,ZTC

#include "Player/STUBaseCharacter.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)// 玩家角色基类的构造函数
    : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(WeaponComponent);
    check(GetCharacterMovement());
    check(GetCapsuleComponent());
    check(GetMesh());

    OnHealthChanged(HealthComponent->GetHealth(), 0.0f);// 初始化健康值改变事件
    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
    // 添加角色落地事件
    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta) {}// 健康值改变时执行的函数,可以在子类中重写

void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

bool ASTUBaseCharacter::IsRunning() const
{
    return false;
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.0f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::OnDeath()
{
    UE_LOG(LogBaseCharacter, Display, TEXT("Player %s is dead"), *GetName());
    // 播放死亡动画
    // 禁用移动
    // 设置生命周期
    // 忽略碰撞
    // 停止开火和瞄准
    // 设置网格组件物理模拟
    // 播放死亡音效
    
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanOnDeath);

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    WeaponComponent->StopFire();
    WeaponComponent->Zoom(false);

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
    // 计算落地速度
    // 根据落地速度造成伤害
{
    const auto FallVelocityZ = -GetVelocity().Z;
    if (FallVelocityZ < LandedDamageVelocity.X) return;

    const auto FallDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    TakeDamage(FallDamage, FPointDamageEvent{}, nullptr, nullptr);

    UE_LOG(LogBaseCharacter, Display, TEXT("Player %s recived landed damage: %f"), *GetName(), FallDamage);
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
    const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if (!MaterialInst) return;
    // 创建并设置动态材质实例
    MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}

void ASTUBaseCharacter::TurnOff()
{
    WeaponComponent->StopFire();
    WeaponComponent->Zoom(false);
    Super::TurnOff();
}

void ASTUBaseCharacter::Reset()
{
    WeaponComponent->StopFire();
    WeaponComponent->Zoom(false);
    Super::Reset();
}
