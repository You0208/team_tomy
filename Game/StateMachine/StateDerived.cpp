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
        // todo �A�j���[�V����
        owner->SetAnimationIndex(owner->Idle_Anim);
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
        owner->SetAnimationIndex(owner->Run_Anim);
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

        //�G�t�F�N�g
        owner->slash->Play(owner->GetPosition(), 1.0f);

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

            owner->CollisionNodeVsEnemies("wepon", "J_wepon", owner->GetAttackCollisionRange());
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
        //�A�j���[�V����
        owner->SetAnimationIndex(next_attack_anim);

        // �U������t���O�I��
        owner->attack_collision_flag = true;

        // �����藚������
        EnemyManager::Instance().HitClear();

        // �X�e�b�v��i�߂�
        attack_step = next_attack_step;

        // ��s���̓t���O����
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
