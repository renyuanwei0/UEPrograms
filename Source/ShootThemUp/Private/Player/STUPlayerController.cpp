// P公司遭遇战,ZTC

#include "Player/STUPlayerController.h"
#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"
#include "STUGameInstance.h"

ASTUPlayerController::ASTUPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");// 在构造函数中创建并添加重生组件
}

void ASTUPlayerController::BeginPlay()
{
    Super::BeginPlay();
    // 在游戏开始时,获取当前世界并获取授权游戏模式
    if (GetWorld())
    {
        if (const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ASTUPlayerController::OnMatchStateChanged);// 添加一个监听器,监听比赛状态的变化
        }
    }
}
// 当比赛状态发生变化时,根据状态切换玩家控制器的输入模式
void ASTUPlayerController::OnMatchStateChanged(ESTUMatchState State)
{
    if (State == ESTUMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly()); // 在比赛进行中时,设置输入模式为游戏模式,隐藏鼠标光标
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());// 在其他状态下,设置输入模式为UI模式,显示鼠标光标
        bShowMouseCursor = true;
    }
}

void ASTUPlayerController::OnPossess(APawn* InPawn)// 当玩家获得新的Pawn时,广播一个OnNewPawn事件
{
    Super::OnPossess(InPawn);

    OnNewPawn.Broadcast(InPawn);
}

void ASTUPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (!InputComponent) return;
    // 绑定暂停游戏和静音音量的输入事件
    InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASTUPlayerController::OnPauseGame);
    InputComponent->BindAction("Mute", IE_Pressed, this, &ASTUPlayerController::OnMuteSound);
}
// 响应暂停游戏的输入事件
void ASTUPlayerController::OnPauseGame()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->SetPause(this);
}
// 响应静音音量的输入事件
void ASTUPlayerController::OnMuteSound()
{
    if (!GetWorld()) return;

    const auto STUGameInstace = GetWorld()->GetGameInstance<USTUGameInstance>();// 获取游戏实例,并切换音量状态
    if (!STUGameInstace) return;

    STUGameInstace->ToggleVolume();
}
