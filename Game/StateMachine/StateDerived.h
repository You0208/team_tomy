// ReSharper disable All
#pragma once
#include"StateBase.h"
#include "Game/Player/Player.h"

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

    class AttackState:public StateBase
    {
    public:
        AttackState(Player* player) :StateBase(player, "AttackState") {}

        void DrawImGui() override;
        void Begin() override;
        void Update() override;
        void End() override;

    private:
        // 先行入力受付開始フレーム
        enum BufferdInputFrame
        {
            First_Judge = 30,
            Second_Judge = 60,

        };
        enum AttackStep
        {
            first_attack,
            second_attack,
            third_attack,
        };
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
        // 先行入力受付可能フラグ
        bool buffered_input_OK = false;

        // 先行入力チェック
        void BufferedInputCheck()
        {
            buffered_input_OK = true;
            if (owner->GetButtonDownB())
                buffered_input = true;
        }

        // 次の攻撃に進む
        void NextStep(int next_attack_step, int next_attack_anim);
    
    };
}
