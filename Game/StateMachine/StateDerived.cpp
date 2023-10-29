#include "StateDerived.h"
#include "imgui.h"
#include "Game/Manager/EnemyManager.h"
#include "Lemur/Input/Input.h"

#include "Game/Player/Player.h"
#include "Game/Scene/ResultScene.h"
#include "Game/Scene/TitleScene.h"
#include "Game/StateMachine/StateMachine.h"
#include "Lemur/Audio/AudioManager.h"
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
        owner->GetModel()->rootMotionFlag = true;
        Lemur::Audio::AudioManager::Instance().play_se(Lemur::Audio::SE::AVOID,false);

        // 入力方向に向く処理
        Turn();
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

    void AvoidState::Turn()
    {
        // 回転行列の作成
        DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(owner->GetAngle().x, owner->GetAngle().y, owner->GetAngle().z);
        // 前方向ベクトル取得
        DirectX::XMVECTOR Forward = rotation_matrix.r[2];

        Forward = DirectX::XMVector3Normalize(Forward);
        DirectX::XMFLOAT3 forward;
        DirectX::XMStoreFloat3(&forward, Forward);

        float input_vec_x = Input::Instance().GetGamePad().GetAxisLX();
        float input_vec_z = Input::Instance().GetGamePad().GetAxisLY();

        DirectX::XMFLOAT3 input_vec=owner->GetMoveVec(input_vec_x,input_vec_z);

        float dot = (input_vec.x * forward.x) + (input_vec.z * forward.z);
        float angle = acosf(dot);

        float cross = (input_vec.x * forward.z) - (input_vec.z * forward.x);
        if(cross<0)
        {
            owner->rotation.y -= angle;
        }
        else
        {
            owner->rotation.y += angle;
        }

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
        switch (attack_step)
        {
        case first_attack:

            // 三段判定
            if (!owner->attack_collision_flag)
            {
                if (owner->GetFrameIndex() == CollisionControlFrame::FirstAttack_Start ||
                    owner->GetFrameIndex() == CollisionControlFrame::FirstAttack_Start + 1 ||
                    owner->GetFrameIndex() == CollisionControlFrame::FirstAttack_Start - 1
                    )
                {
                    //エフェクト再生
                    PlayEffect(DirectX::XMFLOAT3(0.0f, 180.0f, 70.0f),1.5f);
                    //SE再生
                    Lemur::Audio::AudioManager::Instance().play_se(Lemur::Audio::SE::SWING, false);
                    owner->attack_collision_flag = true;
                }
            }

            break;
        case second_attack:
            if (!owner->attack_collision_flag)
            {
                if (owner->GetFrameIndex() == CollisionControlFrame::SecondAttack_Start ||
                    owner->GetFrameIndex() == CollisionControlFrame::SecondAttack_Start + 1 ||
                    owner->GetFrameIndex() == CollisionControlFrame::SecondAttack_Start - 1)
                {
                    // 前方向
                    PlayEffect(DirectX::XMFLOAT3(-30.0f, 180.0f, 0.0f),1.5f);
                    //後ろ方向
                    PlayEffect(DirectX::XMFLOAT3(30.0f, 0.0f, 0.0f),-2.5f);
                    owner->attack_collision_flag = true;
                }
            }
            break;
        case third_attack:

            if (!owner->attack_collision_flag)
            {
                if (owner->GetFrameIndex() == CollisionControlFrame::ThirdAttack_Start ||
                    owner->GetFrameIndex() == CollisionControlFrame::ThirdAttack_Start + 1 ||
                    owner->GetFrameIndex() == CollisionControlFrame::ThirdAttack_Start - 1)
                {
                    PlayEffect(DirectX::XMFLOAT3(0.0f, 180.0f, 90.0f), 1.5f);
                    owner->attack_collision_flag = true;
                }
            }
            if (owner->GetFrameIndex() >= CollisionControlFrame::ThirdAttack_End)
                owner->attack_collision_flag = false;

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

        // 攻撃判定フラグオフ
        owner->attack_collision_flag = false;

        // 当たり履歴消去
        EnemyManager::Instance().HitClear();

        // ステップを進める
        attack_step = next_attack_step;

        // 先行入力フラグ解除
        buffered_input = false;

    }

    void AttackState::PlayEffect(DirectX::XMFLOAT3 rotation,float offset_length)
    {
        /*--------------- エフェクト再生する座標の調整 ---------------*/

        // 武器の座標
        DirectX::XMFLOAT3 wepon_pos = owner->GetModel()->joint_position("sickle", "J_wepon", &owner->keyframe, owner->world);

        // 回転行列の作成
        DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(owner->GetAngle().x, owner->GetAngle().y, owner->GetAngle().z);
        // 前方向ベクトル取得
        DirectX::XMVECTOR Forward = rotation_matrix.r[2];
        DirectX::XMFLOAT3 forward;

        // 前方向ベクトルからちょっと進んだベクトル作成
        Forward = DirectX::XMVector3Normalize(Forward);
        Forward = DirectX::XMVectorScale(Forward, offset_length);

        // そのベクトルをエフェクト再生する座標にたす
        DirectX::XMStoreFloat3(&forward, Forward);
        wepon_pos += forward;

        /*------------------- エフェクト再生 ------------------*/
        Effekseer::Handle handle = owner->slash->Play(wepon_pos, 0.3f);


        /*----------------- 攻撃の角度に傾ける ----------------*/

        DirectX::XMFLOAT3 rot = owner->GetAngle();

        rot.x += DirectX::XMConvertToRadians(rotation.x);
        rot.y += DirectX::XMConvertToRadians(rotation.y);
        rot.z += DirectX::XMConvertToRadians(rotation.z);

        owner->slash->SetRotation(handle, rot);

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

                // エフェクト再生
                owner->parry_spark->Play(owner->GetPosition());
                // 音再生
                Lemur::Audio::AudioManager::Instance().play_se(Lemur::Audio::SE::CONTER, false);

                break;
            }

            if (owner->GetEndAnimation())
                owner->GetStateMachine()->SetNextState(owner->Idle_State);

            break;
        case Attack:

            // アニメーションによる攻撃判定の制限
            if (owner->GetFrameIndex() >= CollisionControlFrame::Attack_Start&&
                owner->GetFrameIndex()<=CollisionControlFrame::Attack_End)
            {
                // 斬撃エフェクト再生
                DirectX::XMFLOAT3 wepon_pos = owner->GetModel()->joint_position("sickle", "J_wepon", &owner->keyframe, owner->world);
                Effekseer::Handle handle = owner->parry_slash->Play(wepon_pos, 0.3f);
                wepon_pos.y += 1.0f;

                DirectX::XMFLOAT3 rot = owner->GetAngle();
                rot.y += DirectX::XMConvertToDegrees(180.0f);
                owner->parry_slash->SetRotation(handle, rot);

                wepon_pos.y += 1.0f;
                owner->attack_collision_flag = true;
                owner->CollisionNodeVsEnemies("sickle", "J_wepon", owner->GetAttackCollisionRange());
            }
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
