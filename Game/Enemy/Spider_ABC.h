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
        // todo 敵の出現位置の設定
        max_health = 300;
        health = max_health;
        attack_power = 20.0f;
        speed_power = 20.0f;

        near_attack_range = 2.0f;
        middle_attack_range = 10.0f;
        scaleFactor = 1.0f;

        // メッシュの名前違うからここで再設定
        meshName = "polySurface1";

        enemy_type = "A_Spider";
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

class Spider_B :
    public Enemy
{
public:
    Spider_B() {}
    Spider_B(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :Enemy(input_, physics_, graphics_)
    {
        position.z = 30;
        rotation.y = DirectX::XMConvertToDegrees(180);

        max_health = 300;
        health = max_health;
        attack_power = 10.0f;
        speed_power = 30.0f;

        near_attack_range = 2.0f;
        middle_attack_range = 10.0f;

        scaleFactor = 1.0f;

        // メッシュの名前違うからここで再設定
        meshName = "polySurface1";

        enemy_type = "B_Spider";
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

class Spider_C :
    public Enemy
{
public:
    Spider_C() {}
    Spider_C(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :Enemy(input_, physics_, graphics_)
    {
        position.z = 30;
        rotation.y = DirectX::XMConvertToDegrees(180);

        max_health = 500;
        health = max_health;
        attack_power = 10.0f;
        speed_power = 20.0f;

        near_attack_range = 2.0f;
        middle_attack_range = 10.0f;

        scaleFactor = 1.0f;

        // メッシュの名前違うからここで再設定
        meshName = "polySurface1";

        enemy_type = "C_Spider";
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

