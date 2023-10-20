#include "StateBase.h"
#include "Game/Player/Player.h"
#include "Lemur/Input/Input.h"
#include "Game/StateMachine/StateMachine.h"
void Nero::Component::AI::StateBase::ChangeJudgeAvoidState()
{
    GamePad& game_pad = Input::Instance().GetGamePad();
    // todo ここのキーは今はテキトー
    if (game_pad.GetButtonDown() & GamePad::BTN_A||
        GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        owner->GetStateMachine()->SetNextState(owner->Avoid_State);
    }

}

void Nero::Component::AI::StateBase::ChangeJudgeDeathState()
{
    if(owner->death)
    {
        owner->GetStateMachine()->SetNextState(owner->Death_State);
    }
}
