#pragma once
#include "Enemy.h"
class Spider_A :
    public Enemy
{
public:
    Spider_A() {}
    Spider_A(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :Enemy(input_, physics_, graphics_)
    {
        rotation.y = 180;

        max_health = 300;
        health = max_health;
        attack_power = 20.0f;
        speed_power = 20.0f;

        near_attack_range = 6.0f;
        middle_attack_range = 10.0f;
        scaleFactor = 3.0f;

        // ���b�V���̖��O�Ⴄ���炱���ōĐݒ�
        meshName = "polySurface1";

        enemy_type = "A_Spider";
    }
    virtual void DrawDebugPrimitive() override;

    void BehaviorTreeInitialize() override;
    void SetUpHitCollision() override;

};

