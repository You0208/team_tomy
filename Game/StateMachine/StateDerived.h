// ReSharper disable All
#pragma once
#include"StateBase.h"

class Player;
namespace Nero::Component::AI
{
    class IdleState :public StateBase
    {
    public:
        IdleState(Player* player) :StateBase(player, "IdleState") {}

        void Begin() override;
        void Update() override;
        void End() override;
    };

    class WalkState :public StateBase
    {
    public:
        WalkState(Player* player) :StateBase(player, "WalkState") {}

        void Begin() override;
        void Update() override;
        void End() override;

    };

    class AvoidState :public StateBase
    {
    public:
        AvoidState(Player* player) :StateBase(player, "AvoidState") {}

        void Begin() override;
        void Update() override;
        void End() override;


    };

    class DeathState :public StateBase
    {
    public:
        DeathState(Player* player) :StateBase(player, "DeathState") {}

        void Begin() override;
        void Update() override;
        void End() override;


    };

    class FearState :public StateBase
    {
    public:
        FearState(Player* player) :StateBase(player, "FearState") {}

        void Begin() override;
        void Update() override;
        void End() override;

    };

    class AttackState:public StateBase
    {
    public:
        AttackState(Player* player) :StateBase(player, "AttackState") {}

        enum AttackStep
        {
            first_attack,
            second_attack,
            third_attack,
        };

        void DrawImGui() override;
        void Begin() override;
        void Update() override;
        void End() override;

        //motion_value_�͉��{���B
        void SetMotionValue(AttackState::AttackStep motion_num, float motion_value_)
        {
            motion_value[motion_num] = motion_value_;
        }

        float GetMotionValue(AttackState::AttackStep motion_num)const { return motion_value[motion_num]; }
        float GetMotionValue(int motion_num)const { return motion_value[motion_num]; }

        int GetAttackStep()const { return attack_step; }

    private:
        // ��s���͎�t�J�n�t���[��
        enum BufferdInputFrame
        {
            First_Judge = 30,
            Second_Judge = 60,

        };
        int attack_step = first_attack;

        // �e���[�V�����l
        float motion_value[3]
        {
            1.0f,
            1.2f,
            1.3f,
        };

        // ��s���͊����t���O
        bool buffered_input = false;
        // ��s���͎�t�\�t���O
        bool buffered_input_OK = false;

        // ��s���̓`�F�b�N
        void BufferedInputCheck();

        // ���̍U���ɐi��
        void NextStep(int next_attack_step, int next_attack_anim);
    
    };

    class SPAttackState :public StateBase
    {
    public:
        SPAttackState(Player* player) :StateBase(player, "FearState") {}

        void Begin() override;
        void Update() override;
        void End() override;

        // 180�t���[���܂ŃJ�E���^�[����󂯕t����
        int can_counter_frame = 180;

        enum Step
        {
            Waiting,// �J�E���^�[�҂�
            Attack,// �J�E���^�[�U��
        };

        float motion_value = 2.5f;
        int step = Waiting;
    };

}
