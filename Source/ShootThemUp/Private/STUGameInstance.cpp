// P公司遭遇战,ZTC

#include "STUGameInstance.h"
#include "Sound/STUSoundFuncLib.h"

void USTUGameInstance::ToggleVolume()
{
    USTUSoundFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}
