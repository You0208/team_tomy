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

        max_health = 300;
        health = max_health;
        attack_power = 100.0f;
        speed_power = 50.0f;

        near_attack_range = 2.0f;
        middle_attack_range = 10.0f;
        scaleFactor = 1.0f;

        // メッシュの名前違うからここで再設定
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

        max_health = 300;
        health = max_health;
        attack_power = 100.0f;
        speed_power = 50.0f;

        near_attack_range = 4.0f;
        middle_attack_range = 10.0f;
        scaleFactor = 1.5f;

        // メッシュの名前違うからここで再設定
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
