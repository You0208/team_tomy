#pragma once
#include "Enemy.h"

class Spider_D :
    public Enemy
{
public:
    Spider_D() {}
    Spider_D(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :Enemy(input_, physics_, graphics_)
    {
        rotation.y = 180;

        max_health = 200;
        health = max_health;
        attack_power = 5.0f;
        speed_power = 20.0f;

        near_attack_range = 2.0f;
        middle_attack_range = 10.0f;
        scaleFactor = 1.0f;

        // メッシュの名前違うからここで再設定
        //meshName = "polySurface1";
        meshName = "spider_middle_spider_middle";

        enemy_type = "D_Spider";
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

class Spider_E :
    public Enemy
{
public:
    Spider_E() {}
    Spider_E(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :Enemy(input_, physics_, graphics_)
    {
        rotation.y = 180;

        max_health = 300;
        health = max_health;
        attack_power = 10.0f;
        speed_power = 50.0f;

        near_attack_range = 2.0f;
        middle_attack_range = 10.0f;
        scaleFactor = 1.0f;

        // メッシュの名前違うからここで再設定
        //meshName = "polySurface1";
        meshName = "spider_middle_spider_middle";

        enemy_type = "E_Spider";
    }

    void BehaviorTreeInitialize() override
    {
        BehaviorTreeInitialize_Level2();
    }
    void SetUpHitCollision() override
    {
        SetUpHitCollisionNormalSize();
    }

};

class Spider_F :
    public Enemy
{
public:
    Spider_F() {}
    Spider_F(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :Enemy(input_, physics_, graphics_)
    {
        rotation.y = 180;

        max_health = 300;
        health = max_health;
        attack_power = 8.0f;
        speed_power = 60.0f;

        near_attack_range = 2.0f;
        middle_attack_range = 10.0f;
        scaleFactor = 1.0f;

        // メッシュの名前違うからここで再設定
        //meshName = "polySurface1";
        meshName = "spider_middle_spider_middle";

        enemy_type = "F_Spider";
    }

    void BehaviorTreeInitialize() override
    {
        BehaviorTreeInitialize_Level2();
    }
    void SetUpHitCollision() override
    {
        SetUpHitCollisionNormalSize();
    }

};
