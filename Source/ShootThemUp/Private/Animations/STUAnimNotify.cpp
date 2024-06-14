// P公司遭遇战,ZTC

#include "Animations/STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    OnNotified.Broadcast(MeshComp);//自定义的委托事件 OnNotified
    Super::Notify(MeshComp, Animation, EventReference);
}
