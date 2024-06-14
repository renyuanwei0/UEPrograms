// P公司遭遇战,ZTC

#include "Components/STUCharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());// 将拥有者转换为ASTUBaseCharacter类型的指针
    return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;// 如果玩家对象存在且正在奔跑,返回最大速度乘以奔跑修正因子;否则返回最大速度
}
