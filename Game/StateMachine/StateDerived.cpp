#include "StateDerived.h"

#include "Lemur/Input/Input.h"

#include "Game/Player/Player.h"
namespace Nero::Component::AI
{
    void IdleState::Begin()
    {
        // todo アニメーション
    }

    void IdleState::Update()
    {
        GamePad game_pad = Input::Instance().GetGamePad();
        float elapsed_time = high_resolution_timer::Instance().time_interval();


        //if(owner->InputMove(elapsed_time))
        //{
        //    owner->GetStateMachine()->SetNextState(owner->WalkAction);
        //}
    }

    void IdleState::End()
    {
    }



    void WalkState::Begin()
    {
        // todo アニメーション
        //owner->anim_operator->SetAnimationIndex(owner->Walk);
    }

    void WalkState::Update()
    {
        GamePad game_pad = Input::Instance().GetGamePad();
        float elapsed_time = high_resolution_timer::Instance().time_interval();

        //if (!owner->InputMove(elapsed_time))
        //{
        //    owner->GetStateMachine()->SetNextState(owner->IdleAction);
        //}

    }

    void WalkState::End()
    {
    }
}
