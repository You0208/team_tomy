#include "StateDerived.h"

#include "Lemur/Input/Input.h"

#include "Game/Player/Player.h"
#include "Game/StateMachine/StateMachine.h"
namespace Nero::Component::AI
{
    void IdleState::Begin()
    {
        // todo アニメーション
        owner->SetAnimationIndex(owner->Idle_Anim);
    }

    void IdleState::Update()
    {
        GamePad game_pad = Input::Instance().GetGamePad();
        float elapsed_time = high_resolution_timer::Instance().time_interval();

        // 入力があれば回避ステートに移行する
        ChangeJudgeAvoidState();

        if(owner->InputMove())
        {
            owner->GetStateMachine()->SetNextState(owner->Walk_State);
        }
    }

    void IdleState::End()
    {
    }



    void WalkState::Begin()
    {
        // todo アニメーション
        owner->SetAnimationIndex(owner->Run_Anim);
    }

    void WalkState::Update()
    {
        GamePad game_pad = Input::Instance().GetGamePad();

        // 入力があれば回避ステートに移行する
        ChangeJudgeAvoidState();

        if (!owner->InputMove())
        {
            owner->GetStateMachine()->SetNextState(owner->Idle_State);
        }

    }

    void WalkState::End()
    {
    }

    void AvoidState::Begin()
    {
        owner->SetAnimationIndex(owner->Avoid_Anim);
    }

    void AvoidState::Update()
    {
        if(owner->GetEndAnimation())
        {
            owner->GetStateMachine()->SetNextState(owner->Idle_State);
        }
    }

    void AvoidState::End()
    {
    }
}
