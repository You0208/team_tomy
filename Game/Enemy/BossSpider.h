#pragma once
#include "Enemy.h"
#include "Enemy.h"
class BossSpider :
    public Enemy
{
public:
    BossSpider() {}
    BossSpider(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :Enemy(input_, physics_, graphics_)
    {
        position.z += 15;
        rotation.y = 180;
        max_health = 5000;
        health = max_health;

        attack_power = 50.0f;
        speed_power = 70.0f;
        near_attack_range = 6.0f;
        middle_attack_range = 10.0f;
        scaleFactor = 3.0f;

        // ÉÅÉbÉVÉÖÇÃñºëOà·Ç§Ç©ÇÁÇ±Ç±Ç≈çƒê›íË
        meshName = "spider_boss_spider_boss";

        enemy_type = "BossSpider";
    }


    void BehaviorTreeInitialize() override;
    void SetUpHitCollision() override;
};

