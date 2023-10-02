#include "StateBase.h"
#include "Game/Player/Player.h"
#include "Lemur/Input/Input.h"
#include "Game/StateMachine/StateMachine.h"
void Nero::Component::AI::StateBase::ChangeJudgeAvoidState()
{
    GamePad& game_pad = Input::Instance().GetGamePad();
    // todo ここのキーは今はテキトー
    if (game_pad.GetButtonDown() & GamePad::BTN_A)
    {
        owner->GetStateMachine()->SetNextState(owner->Avoid_State);
    }

}
