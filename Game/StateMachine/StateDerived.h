// ReSharper disable All
#pragma once
#include"StateBase.h"

class Player;
#include <DirectXMath.h>
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

        void Turn();

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
        enum CollisionControlFrame
        {
            FirstAttack_Start=25,
            SecondAttack_Start=55,
            ThirdAttack_Start=45,
            ThirdAttack_End=68,
        };

        //  �A�j���[�V��������U�����莞�Ԃ����߂�
        void SetAttackCollisionFrag();

        // �����ڂ�
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
        // ��s���͎�t�\�t���O(ImGui�Ō���p)
        bool buffered_input_OK = false;

        // ��s���̓`�F�b�N
        void BufferedInputCheck();

        // ���̍U���ɐi��
        void NextStep(int next_attack_step, int next_attack_anim);

        // �a���ɍ��킹����]�l�������ɓ����
        //�������ɂ͕���̍��W����O�����ɂǂꂾ�����ꂽ�ʒu�ɍĐ����邩
        void PlayEffect(DirectX::XMFLOAT3 rotation,float offset_length);
    };

    class SPAttackState :public StateBase
    {
    public:
        SPAttackState(Player* player) :StateBase(player, "SPAttackState") {}

        void Begin() override;
        void Update() override;
        void End() override;

        // 180�t���[���܂ŃJ�E���^�[����󂯕t����
        int can_counter_frame = 95;

        enum CollisionControlFrame
        {
            Attack_Start = 35,
            Attack_End = 65,
        };

        enum Step
        {
            Waiting,// �J�E���^�[�҂�
            Attack,// �J�E���^�[�U��
        };


    private:

        float motion_value = 2.5f;
        int step = Waiting;

        //���[�V�����l�̌v�Z,�ݒ�
        void CalcMotionValue();
    };

}
