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
        // todo �A�j���[�V����
        owner->SetAnimationIndex(owner->Idle_Anim, true);
    }

    void IdleState::Update()
    {
        GamePad game_pad = Input::Instance().GetGamePad();
        float elapsed_time = high_resolution_timer::Instance().time_interval();

        // ���͂�����Ή���X�e�[�g�Ɉڍs����
        ChangeJudgeAvoidState();

        // ���񂾂玀�S�X�e�[�g�Ɉڍs����
        ChangeJudgeDeathState();

        // ���͂�����Έړ��X�e�[�g
        if(owner->InputMove())
        {
            owner->GetStateMachine()->SetNextState(owner->Run_State);
        }
        // �U���{�^�������ꂽ��U���X�e�[�g
        if (owner->GetButtonDownB_AND_MouseLeft())
        {
            owner->GetStateMachine()->SetNextState(owner->Attack_State);
        }
        // ����U���{�^�������ꂽ�����U���X�e�[�g
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
        // todo �A�j���[�V����
        owner->SetAnimationIndex(owner->Run_Anim, true);
    }

    void WalkState::Update()
    {
        GamePad game_pad = Input::Instance().GetGamePad();

        // ���͂�����Ή���X�e�[�g�Ɉڍs����
        ChangeJudgeAvoidState();

        // ���񂾂玀�S�X�e�[�g�Ɉڍs����
        ChangeJudgeDeathState();

        // �~�܂��Ă���ҋ@�X�e�[�g
        if (!owner->InputMove())
        {
            owner->GetStateMachine()->SetNextState(owner->Idle_State);
        }
        // �U���{�^�������ꂽ��U���X�e�[�g
        if (owner->GetButtonDownB_AND_MouseLeft())
        {
            owner->GetStateMachine()->SetNextState(owner->Attack_State);
        }
        // ����U���{�^�������ꂽ�����U���X�e�[�g
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
    }

    void AvoidState::Update()
    {
        if(owner->GetEndAnimation())
        {
            owner->GetStateMachine()->SetNextState(owner->Idle_State);
        }

        // ���񂾂玀�S�X�e�[�g�Ɉڍs����
        ChangeJudgeDeathState();

        // todo �����̖��G���Ԓ���
        // ����A�j���[�V�����J�n���疳�G�t���[���܂ł̊Ԗ��G
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
        // todo ���S��̏����ǂ����邩
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
        // ��ʐU��
        Camera::Instance().ScreenVibrate(0.05f, 0.3f);

        owner->SetAnimationIndex(owner->Fear_Anim);
    }

    void FearState::Update()
    {
        // ���ݒ��͖��G
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
        // �p�x�␳
        owner->AttackAngleInterpolation();

        owner->SetAnimationIndex(owner->FirstAttack_Anim);

        owner->attack_collision_flag = false;

        attack_step = first_attack;


        EnemyManager::Instance().HitClear();

    }

    void AttackState::Update()
    {

        SetAttackCollisionFrag();

        // ���񂾂玀�S�X�e�[�g�Ɉڍs����
        ChangeJudgeDeathState();

        // �����蔻��J�n���ĂĂ܂��������ĂȂ������画�肷��
        if (owner->attack_collision_flag)
        {
            // todo ������Ɖ�肭�ǂ�����ς�����
            // ���[�V�����l�ݒ�
            owner->motion_value = motion_value[attack_step];

            owner->CollisionNodeVsEnemies("sickle", "J_wepon", owner->GetAttackCollisionRange());
        }
        
        // ����U���{�^�������ꂽ�����U���X�e�[�g
        if (owner->GetButtonDownY_AND_MouseRight())
        {
            if (owner->can_SP_attack)
                owner->GetStateMachine()->SetNextState(owner->SPAttack_State);
        }

        switch (attack_step)
        {
        case first_attack:


            // todo ������s���͎�t���ԍl����

            // �U���t���[����������x�i�񂾂��s���̓`�F�b�N
            if (owner->GetFrameIndex() > BufferdInputFrame::First_Judge)
            {
                BufferedInputCheck();
            }
            else buffered_input_OK = false;

            // �A�j���[�V�������I�������
            if (owner->GetEndAnimation())
            {
                // ��s���͂�����Ύ��̍U���ɍs��
                if (buffered_input)
                {
                    NextStep(second_attack,owner->SecondAttack_Anim);
                }
                // ��s���͂��Ȃ���ΏI��
                else
                    owner->GetStateMachine()->SetNextState(owner->Idle_State);
            }

            break;
        case second_attack:

            // �U���t���[����������x�i�񂾂��s���̓`�F�b�N
            if (owner->GetFrameIndex() > BufferdInputFrame::Second_Judge)
            {
                BufferedInputCheck();
            }
            else buffered_input_OK = false;


            // �A�j���[�V�������I�������
            if (owner->GetEndAnimation())
            {
                // ��s���͂�����Ύ��̍U���ɍs��
                if (buffered_input)
                {
                    NextStep(third_attack, owner->ThirdAttack_Anim);
                }
                // ��s���͂��Ȃ���ΏI��
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

            // �O�i����
            if (!owner->attack_collision_flag)
            {
                if (owner->GetFrameIndex() == CollisionControlFrame::FirstAttack_Start ||
                    owner->GetFrameIndex() == CollisionControlFrame::FirstAttack_Start + 1 ||
                    owner->GetFrameIndex() == CollisionControlFrame::FirstAttack_Start - 1
                    )
                {
                    //�G�t�F�N�g�Đ�
                    PlayEffect(DirectX::XMFLOAT3(0.0f, 180.0f, 70.0f),1.5f);
                    //SE�Đ�
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
                    // �O����
                    PlayEffect(DirectX::XMFLOAT3(-30.0f, 180.0f, 0.0f),1.5f);
                    //������
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
        //�A�j���[�V����
        owner->SetAnimationIndex(next_attack_anim);

        // �U������t���O�I�t
        owner->attack_collision_flag = false;

        // �����藚������
        EnemyManager::Instance().HitClear();

        // �X�e�b�v��i�߂�
        attack_step = next_attack_step;

        // ��s���̓t���O����
        buffered_input = false;

    }

    void AttackState::PlayEffect(DirectX::XMFLOAT3 rotation,float offset_length)
    {
        /*--------------- �G�t�F�N�g�Đ�������W�̒��� ---------------*/

        // ����̍��W
        DirectX::XMFLOAT3 wepon_pos = owner->GetModel()->joint_position("sickle", "J_wepon", &owner->keyframe, owner->world);

        // ��]�s��̍쐬
        DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(owner->GetAngle().x, owner->GetAngle().y, owner->GetAngle().z);
        // �O�����x�N�g���擾
        DirectX::XMVECTOR Forward = rotation_matrix.r[2];
        DirectX::XMFLOAT3 forward;

        // �O�����x�N�g�����炿����Ɛi�񂾃x�N�g���쐬
        Forward = DirectX::XMVector3Normalize(Forward);
        Forward = DirectX::XMVectorScale(Forward, offset_length);

        // ���̃x�N�g�����G�t�F�N�g�Đ�������W�ɂ���
        DirectX::XMStoreFloat3(&forward, Forward);
        wepon_pos += forward;

        /*------------------- �G�t�F�N�g�Đ� ------------------*/
        Effekseer::Handle handle = owner->slash->Play(wepon_pos, 0.3f);


        /*----------------- �U���̊p�x�ɌX���� ----------------*/

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
                //�J�E���^�[��t�J�n
                owner->SetCanCounter(true);
            }
            else
            {
                //�J�E���^�[��t�I��
                owner->SetCanCounter(false);
            }

            // �J�E���^�[����������X�e�b�v��i�߂�
            if (owner->GetIsCounter())
            {
                owner->SetCanCounter(false);
                owner->SetAnimationIndex(owner->CounterAttack_Anim);
                step = Attack;

                //���[�V�����l�̐ݒ�
                CalcMotionValue();

                // �J�E���^�[���͖��G�ɂ���
                owner->invincible = true;

                // �G�t�F�N�g�Đ�
                owner->parry_spark->Play(owner->GetPosition());
                // ���Đ�
                Lemur::Audio::AudioManager::Instance().play_se(Lemur::Audio::SE::CONTER, false);

                break;
            }

            if (owner->GetEndAnimation())
                owner->GetStateMachine()->SetNextState(owner->Idle_State);

            break;
        case Attack:

            // �A�j���[�V�����ɂ��U������̐���
            if (owner->GetFrameIndex() >= CollisionControlFrame::Attack_Start&&
                owner->GetFrameIndex()<=CollisionControlFrame::Attack_End)
            {
                // �a���G�t�F�N�g�Đ�
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
        //�X�L�� [����] �� [�Z�p] �������Ă��烂�[�V�����l1.5�{
        if (owner->HaveSkill("Frenzy") || owner->HaveSkill("Technique"))
        {
            // todo �������ɂ�肷���H
            // ������Ƃ������Ă���4.0�{
            if (owner->HaveSkill("Frenzy") && owner->HaveSkill("Technique"))
                owner->motion_value = motion_value * 4.0f;
            else
                owner->motion_value = motion_value * 1.5f;
        }
        // �ǂ���������ĂȂ�������ʏ�̃��[�V�����l
        else owner->motion_value = motion_value;

    }
}
