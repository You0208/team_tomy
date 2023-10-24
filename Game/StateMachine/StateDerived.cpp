#include "StateDerived.h"
#include "imgui.h"
#include "Game/Manager/EnemyManager.h"
#include "Lemur/Input/Input.h"

#include "Game/Player/Player.h"
#include "Game/Scene/ResultScene.h"
#include "Game/Scene/TitleScene.h"
#include "Game/StateMachine/StateMachine.h"
#include "Lemur/Graphics/Camera.h"
#include "Lemur/Scene/SceneManager.h"

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

        // 死んだら死亡ステートに移行する
        ChangeJudgeDeathState();

        // 入力があれば移動ステート
        if(owner->InputMove())
        {
            owner->GetStateMachine()->SetNextState(owner->Run_State);
        }
        // 攻撃ボタン押されたら攻撃ステート
        if (owner->GetButtonDownB_AND_MouseLeft())
        {
            owner->GetStateMachine()->SetNextState(owner->Attack_State);
        }
        // 特殊攻撃ボタン押されたら特殊攻撃ステート
        if (owner->GetButtonDownY_AND_MouseRight())
        {
            if (owner->can_SP_attack)
            owner->GetStateMachine()->SetNextState(owner->SPAttack_State);
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

        // 死んだら死亡ステートに移行する
        ChangeJudgeDeathState();

        // 止まってたら待機ステート
        if (!owner->InputMove())
        {
            owner->GetStateMachine()->SetNextState(owner->Idle_State);
        }
        // 攻撃ボタン押されたら攻撃ステート
        if (owner->GetButtonDownB_AND_MouseLeft())
        {
            owner->GetStateMachine()->SetNextState(owner->Attack_State);
        }
        // 特殊攻撃ボタン押されたら特殊攻撃ステート
        if (owner->GetButtonDownY_AND_MouseRight())
        {
            if (owner->can_SP_attack)
            owner->GetStateMachine()->SetNextState(owner->SPAttack_State);
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

        // 死んだら死亡ステートに移行する
        ChangeJudgeDeathState();

        // todo ここの無敵時間調整
        // 回避アニメーション開始から無敵フレームまでの間無敵
        if (owner->GetFrameIndex() <= owner->GetInvincibleFrame())
            owner->invincible = true;
        else
            owner->invincible = false;
    }

    void AvoidState::End()
    {
        owner->invincible = false;
    }

    void DeathState::Begin()
    {
        owner->invincible = true;
        owner->SetAnimationIndex(owner->Death_Anim);
    }

    void DeathState::Update()
    {
        // todo 死亡後の処理どうするか
        if(owner->GetEndAnimation())
        {
            owner->down_count--;
            owner->death_anim_end = true;

        }
    }

    void DeathState::End()
    {
        
    }

    void FearState::Begin()
    {
        // 画面振動
        Camera::Instance().ScreenVibrate(0.05f, 0.3f);

        owner->SetAnimationIndex(owner->Fear_Anim);
    }

    void FearState::Update()
    {
        // 怯み中は無敵
        owner->invincible = true;
        if (owner->GetEndAnimation())
            owner->GetStateMachine()->SetNextState(owner->Idle_State);
    }

    void FearState::End()
    {
        owner->invincible = false;
    }


    void AttackState::DrawImGui()
    {
        ImGui::DragInt("attack_step", &attack_step);
        ImGui::Checkbox("buffered_input", &buffered_input);
        ImGui::Checkbox("buffered_input_OK", &buffered_input_OK);
        ImGui::DragFloat("motion_value", &motion_value[attack_step]);
    }

    void AttackState::Begin()
    {
        // 角度補正
        owner->AttackAngleInterpolation();

        owner->SetAnimationIndex(owner->FirstAttack_Anim);

        owner->attack_collision_flag = false;

        attack_step = first_attack;


        EnemyManager::Instance().HitClear();

        //エフェクト
        owner->slash->Play(owner->GetPosition(), 1.0f);

    }

    void AttackState::Update()
    {

        SetAttackCollisionFrag();

        // 死んだら死亡ステートに移行する
        ChangeJudgeDeathState();

        // 当たり判定開始しててまだ当たってなかったら判定する
        if (owner->attack_collision_flag)
        {
            // todo ちょっと回りくどいから変えたい
            // モーション値設定
            owner->motion_value = motion_value[attack_step];

            owner->CollisionNodeVsEnemies("sickle", "J_wepon", owner->GetAttackCollisionRange());
        }
        
        // 特殊攻撃ボタン押されたら特殊攻撃ステート
        if (owner->GetButtonDownY_AND_MouseRight())
        {
            if (owner->can_SP_attack)
                owner->GetStateMachine()->SetNextState(owner->SPAttack_State);
        }

        switch (attack_step)
        {
        case first_attack:


            // todo ここ先行入力受付時間考える

            // 攻撃フレームがある程度進んだら先行入力チェック
            if (owner->GetFrameIndex() > BufferdInputFrame::First_Judge)
            {
                BufferedInputCheck();
            }
            else buffered_input_OK = false;

            // アニメーションが終わったら
            if (owner->GetEndAnimation())
            {
                // 先行入力があれば次の攻撃に行く
                if (buffered_input)
                {
                    NextStep(second_attack,owner->SecondAttack_Anim);
                }
                // 先行入力がなければ終了
                else
                    owner->GetStateMachine()->SetNextState(owner->Idle_State);
            }

            break;
        case second_attack:

            // 攻撃フレームがある程度進んだら先行入力チェック
            if (owner->GetFrameIndex() > BufferdInputFrame::Second_Judge)
            {
                BufferedInputCheck();
            }
            else buffered_input_OK = false;


            // アニメーションが終わったら
            if (owner->GetEndAnimation())
            {
                // 先行入力があれば次の攻撃に行く
                if (buffered_input)
                {
                    NextStep(third_attack, owner->ThirdAttack_Anim);
                }
                // 先行入力がなければ終了
                else
                    owner->GetStateMachine()->SetNextState(owner->Idle_State);
            }


        case third_attack:

            if (owner->GetEndAnimation())
                owner->GetStateMachine()->SetNextState(owner->Idle_State);

        }
    }

    void AttackState::End()
    {
        owner->attack_collision_flag = false;
        EnemyManager::Instance().HitClear();
    }

    void AttackState::SetAttackCollisionFrag()
    {
        owner->attack_collision_flag = false;
        switch (attack_step)
        {
        case first_attack:
            
            if (owner->GetFrameIndex() >= CollisionControlFrame::FirstAttack_Start)
                owner->attack_collision_flag = true;

            break;
        case second_attack:
            
            if (owner->GetFrameIndex() >= CollisionControlFrame::SecondAttack_Start)
                owner->attack_collision_flag = true;

            break;
        case third_attack:

            if (owner->GetFrameIndex() >= CollisionControlFrame::ThirdAttack_Start &&
                owner->GetFrameIndex() <= CollisionControlFrame::ThirdAttack_End)
                owner->attack_collision_flag = true;

            break;
        }
    }

    void AttackState::BufferedInputCheck()
    {
        buffered_input_OK = true;
        if (owner->GetButtonDownB_AND_MouseLeft())
            buffered_input = true;
    }

    void AttackState::NextStep(int next_attack_step, int next_attack_anim)
    {
        //アニメーション
        owner->SetAnimationIndex(next_attack_anim);

        // 攻撃判定フラグオン
        owner->attack_collision_flag = true;

        // 当たり履歴消去
        EnemyManager::Instance().HitClear();

        // ステップを進める
        attack_step = next_attack_step;

        // 先行入力フラグ解除
        buffered_input = false;

    }

    void SPAttackState::Begin()
    {
        EnemyManager::Instance().HitClear();

        owner->SetAnimationIndex(owner->Counter_Anim);
    }

    void SPAttackState::Update()
    {

        switch (step)
        {
        case Waiting:
            if (owner->GetFrameIndex() <= can_counter_frame)
            {
                //カウンター受付開始
                owner->SetCanCounter(true);
            }
            else
            {
                //カウンター受付終了
                owner->SetCanCounter(false);
            }

            // カウンター成功したらステップを進める
            if (owner->GetIsCounter())
            {
                owner->SetCanCounter(false);
                owner->SetAnimationIndex(owner->CounterAttack_Anim);
                step = Attack;

                //モーション値の設定
                CalcMotionValue();

                // カウンター中は無敵にする
                owner->invincible = true;
                break;
            }

            if (owner->GetEndAnimation())
                owner->GetStateMachine()->SetNextState(owner->Idle_State);

            break;
        case Attack:

            owner->CollisionNodeVsEnemies("wepon", "J_wepon", owner->GetAttackCollisionRange());

            if (owner->GetEndAnimation())
                owner->GetStateMachine()->SetNextState(owner->Idle_State);
            break;
        }
    }

    void SPAttackState::End()
    {
        EnemyManager::Instance().HitClear();

        step = Waiting;
        owner->SetIsCounter(false);
        owner->SetCanCounter(false);
        owner->invincible = false;

        owner->can_SP_attack = false;
        owner->ResetSPAttackTimer();
    }

    void SPAttackState::CalcMotionValue()
    {
        //スキル [狂乱] か [技術] を持ってたらモーション値1.5倍
        if (owner->HaveSkill("Frenzy") || owner->HaveSkill("Technique"))
        {
            // todo さすがにやりすぎ？
            // もし二つとも持ってたら4.0倍
            if (owner->HaveSkill("Frenzy") && owner->HaveSkill("Technique"))
                owner->motion_value = motion_value * 4.0f;
            else
                owner->motion_value = motion_value * 1.5f;
        }
        // どちらも持ってなかったら通常のモーション値
        else owner->motion_value = motion_value;

    }
}
