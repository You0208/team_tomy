#pragma once
#include "Enemy.h"

class Spider_G :
    public Enemy
{
public:
    Spider_G() {}
    Spider_G(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :Enemy(input_, physics_, graphics_)
    {
        rotation.y = 180;

        max_health = 200;
        health = max_health;
        attack_power = 8.0f;
        speed_power = 50.0f;

        near_attack_range = 2.0f;
        middle_attack_range = 10.0f;
        scaleFactor = 1.0f;

        // ���b�V���̖��O�Ⴄ���炱���ōĐݒ�
        meshName = "polySurface1";
        meshName = "spider_middle_spider_middle";

        enemy_type = "G_Spider";
    }

    void BehaviorTreeInitialize() override
    {
        BehaviorTreeInitialize_Level3();
    }
    void SetUpHitCollision() override
    {
        SetUpHitCollisionNormalSize();
    }

};

class Spider_H :
    public Enemy
{
public:
    Spider_H() {}
    Spider_H(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :Enemy(input_, physics_, graphics_)
    {
        rotation.y = 180;

        max_health = 1000;
        health = max_health;
        attack_power = 15.0f;
        speed_power = 80.0f;

        near_attack_range = 4.0f;
        middle_attack_range = 10.0f;
        scaleFactor = 1.5f;

        // ���b�V���̖��O�Ⴄ���炱���ōĐݒ�
        meshName = "polySurface1";
        meshName = "spider_middle_spider_middle";

        enemy_type = "H_Spider";
    }

    void BehaviorTreeInitialize() override
    {
        BehaviorTreeInitialize_Level4();
    }
    void SetUpHitCollision() override
    {
        SetUpHitCollisionNormalSize();
    }

};
