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

        //motion_value_は何倍か。
        void SetMotionValue(AttackState::AttackStep motion_num, float motion_value_)
        {
            motion_value[motion_num] = motion_value_;
        }

        float GetMotionValue(AttackState::AttackStep motion_num)const { return motion_value[motion_num]; }
        float GetMotionValue(int motion_num)const { return motion_value[motion_num]; }

        int GetAttackStep()const { return attack_step; }

    private:
        // 先行入力受付開始フレーム
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

        //  アニメーションから攻撃判定時間を決める
        void SetAttackCollisionFrag();

        // 何撃目か
        int attack_step = first_attack;

        // 各モーション値
        float motion_value[3]
        {
            1.0f,
            1.2f,
            1.3f,
        };

        // 先行入力完了フラグ
        bool buffered_input = false;
        // 先行入力受付可能フラグ(ImGuiで見る用)
        bool buffered_input_OK = false;

        // 先行入力チェック
        void BufferedInputCheck();

        // 次の攻撃に進む
        void NextStep(int next_attack_step, int next_attack_anim);

        // 斬撃に合わせた回転値を引数に入れる
        //第二引数には武器の座標から前方向にどれだけずれた位置に再生するか
        void PlayEffect(DirectX::XMFLOAT3 rotation,float offset_length);
    };

    class SPAttackState :public StateBase
    {
    public:
        SPAttackState(Player* player) :StateBase(player, "SPAttackState") {}

        void Begin() override;
        void Update() override;
        void End() override;

        // 180フレームまでカウンター判定受け付ける
        int can_counter_frame = 95;

        enum CollisionControlFrame
        {
            Attack_Start = 35,
            Attack_End = 65,
        };

        enum Step
        {
            Waiting,// カウンター待ち
            Attack,// カウンター攻撃
        };


    private:

        float motion_value = 2.5f;
        int step = Waiting;

        //モーション値の計算,設定
        void CalcMotionValue();
    };

}
