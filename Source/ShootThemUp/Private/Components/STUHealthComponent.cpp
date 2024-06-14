// P公司遭遇战,ZTC

#include "Components/STUHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"
#include "STUGameModeBase.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Perception/AISense_Damage.h"
#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);

    SetHealth(MaxHealth);
    // 获取组件所属的Actor,并绑定伤害事件
    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
        ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthComponent::OnTakePointDamage);
        ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthComponent::OnTakeRadialDamage);
    }
}
// 收到点伤害时的回调函数
void USTUHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
    class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType,
    AActor* DamageCauser)
{
    const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
    // UE_LOG(LogHealthComponent, Display, TEXT("On point damage: %f, final damage: %f, bone: %s"), Damage, FinalDamage,
    // *BoneName.ToString());
    ApplyDamage(FinalDamage, InstigatedBy);
}
// 收到范围伤害时的回调函数
void USTUHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin,
    const FHitResult&, class AController* InstigatedBy, AActor* DamageCauser)
{
    // UE_LOG(LogHealthComponent, Display, TEXT("On radial damage: %f"), Damage);
    ApplyDamage(Damage, InstigatedBy);
}
// 收到任何伤害时的回调函数
void USTUHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    // UE_LOG(LogHealthComponent, Display, TEXT("On any damage: %f"), Damage);
}

void USTUHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld()) return;// 如果伤害值小于等于0,玩家已死亡,或者世界不存在,则返回

    SetHealth(Health - Damage);
    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);// 清除治疗计时器

    if (IsDead())// 如果玩家已死亡,调用Killed函数并广播OnDeath事件
    {
        Killed(InstigatedBy);
        OnDeath.Broadcast();
    }
    else if (AutoHeal)// 否则如果开启自动治疗,启动治疗计时器
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }

    PlayCameraShake(); // 播放摄像机震动效果
    ReportDamageEvent(Damage, InstigatedBy);// 报告伤害事件
}

void USTUHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);// 治疗更新函数

    if (IsHealthFull() && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);// 限制生命值在0到最大生命值之间
    const auto HealthDelta = NextHealth - Health;

    Health = NextHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta);// 广播生命值改变事件
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
    if (IsDead() || IsHealthFull()) return false;

    SetHealth(Health + HealthAmount);
    return true;
}

bool USTUHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}
// 播放摄像机震动效果的函数
void USTUHealthComponent::PlayCameraShake()
{
    if (IsDead()) return;

    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager) return;

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void USTUHealthComponent::Killed(AController* KillerController)// 玩家死亡时的处理函数
{
    if (!GetWorld()) return;

    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    const auto Player = Cast<APawn>(GetOwner());
    const auto VictimController = Player ? Player->Controller : nullptr;

    GameMode->Killed(KillerController, VictimController);
}

float USTUHealthComponent::GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName)// 获取点伤害修正系数的函数
{
    const auto Character = Cast<ACharacter>(DamagedActor);// 获取受伤角色,并检查是否有有效的骨骼信息
    if (!Character ||             //
        !Character->GetMesh() ||  //
        !Character->GetMesh()->GetBodyInstance(BoneName))
        return 1.0f;

    const auto PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
    if (!PhysMaterial || !DamageModifiers.Contains(PhysMaterial)) return 1.0f;

    return DamageModifiers[PhysMaterial];
}

void USTUHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
    if (!InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;

    UAISense_Damage::ReportDamageEvent(GetWorld(),    //
        GetOwner(),                                   //
        InstigatedBy->GetPawn(),                      //
        Damage,                                       //
        InstigatedBy->GetPawn()->GetActorLocation(),  //
        GetOwner()->GetActorLocation());
}
