#include "StateDerived.h"
#include "imgui.h"
#include "Game/Manager/EnemyManager.h"
#include "Lemur/Input/Input.h"

#include "Game/Player/Player.h"
#include "Game/Scene/TitleScene.h"
#include "Game/StateMachine/StateMachine.h"
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


        if(owner->InputMove())
        {
            owner->GetStateMachine()->SetNextState(owner->Run_State);
        }

        if(owner->GetButtonDownB_AND_MouseLeft())
        {
            owner->GetStateMachine()->SetNextState(owner->Attack_State);
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

        if (!owner->InputMove())
        {
            owner->GetStateMachine()->SetNextState(owner->Idle_State);
        }
        if (owner->GetButtonDownB_AND_MouseLeft())
        {
            owner->GetStateMachine()->SetNextState(owner->Attack_State);
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
        owner->SetAnimationIndex(owner->Death_Anim,false);
    }

    void DeathState::Update()
    {
        // todo ���S��̏����ǂ����邩
        if(owner->GetEndAnimation())
        {
            owner->down_count--;
            if(owner->down_count<0)
            {
                //Lemur::Scene::SceneManager::Instance().ChangeScene(new TitleScene);
            }
            else
            {
                // todo ���U���g�V�[��
            }
        }
    }

    void DeathState::End()
    {
    }

    void FearState::Begin()
    {
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
        owner->SetAnimationIndex(owner->FirstAttack_Anim);

        // todo �A�j���[�V��������U�����莞�Ԃ����߂�
        owner->attack_collision_flag = true;

        attack_step = first_attack;


        EnemyManager::Instance().HitClear();
    }

    void AttackState::Update()
    {
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
        // todo �Ă���HP�ւ炷

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
}
