#pragma once
#include "Enemy.h"
class BossSpider :
    public Enemy
{
public:
    BossSpider() {}
    BossSpider(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :Enemy(input_, physics_, graphics_,EnemyType::BossSpider)
    {
        max_health = 5000;
        health = max_health;

        near_attack_range = 3.0f;

    }

    virtual void DrawDebugPrimitive() override;

    void BehaviorTreeInitialize() override;
};

