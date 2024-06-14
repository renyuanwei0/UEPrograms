// P公司遭遇战,ZTC

#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ASTULauncherWeapon::StartFire()
{
    Super::StartFire();

    MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
    if (!GetWorld()) return;

    if (IsAmmoEmpty())// 检查是否弹药耗尽
    {
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation()); // 播放无弹药声音
        return;
    }

    FVector TraceStart, TraceEnd;// 获取射线追踪的起点和终点
    if (!GetTraceData(TraceStart, TraceEnd)) return;
    // 进行射线追踪检测
    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;// 确定射击终点
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();// 计算射击方向

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());// 设置生成投射物的变换
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);// 生成投射物
    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());    // 设置投射物的拥有者
        Projectile->FinishSpawning(SpawnTransform);    // 完成投射物的生成
    }

    DecreaseAmmo();
    SpawnMuzzleFX();// 生成枪口特效
    UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);// 播放开火声音

    // just for setting IsFiring to false
    StopFire();
}
