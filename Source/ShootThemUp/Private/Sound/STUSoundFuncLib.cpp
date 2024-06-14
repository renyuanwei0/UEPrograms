// P公司遭遇战,ZTC

#include "Sound/STUSoundFuncLib.h"
#include "Sound/SoundClass.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUSoundFuncLib, All, All);

void USTUSoundFuncLib::SetSoundClassVolume(USoundClass* SoundClass, float Volume)
{
    if (!SoundClass) return;

    SoundClass->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);// 将音量值限制在0.0到1.0之间
    UE_LOG(LogSTUSoundFuncLib, Display, TEXT("Sound class volume was changed: %s = %f"), *SoundClass->GetName(),
        SoundClass->Properties.Volume);
}

void USTUSoundFuncLib::ToggleSoundClassVolume(USoundClass* SoundClass)
{
    if (!SoundClass) return;
    // 如果当前音量大于0,则设置为0,否则设置为1
    const auto NextVolume = SoundClass->Properties.Volume > 0.0f ? 0.0f : 1.0f;
    SetSoundClassVolume(SoundClass, NextVolume);
}
