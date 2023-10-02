#pragma once
#include"StateBase.h"

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
}
