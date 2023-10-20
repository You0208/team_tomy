#pragma once
#include "Enemy.h"
#include "Enemy.h"
class SmallSpider :
    public Enemy
{
public:
    SmallSpider() {}
    SmallSpider(InputComponent* input_,
        PhysicsComponent* physics_,
        GraphicsComponent* graphics_) :Enemy(input_, physics_, graphics_)
    {
        max_health = 10;
        speed_power = 20;
        health = max_health;
        attack_power = 5.0f;

        meshName = "polySurface1";
        enemy_type = "SmallSpider";
    }

    void BehaviorTreeInitialize() override
    {
        BehaviorTreeInitialize_Level1();
    }

    void SetUpHitCollision() override
    {
        auto PutInCollisions = [this](const char* bone_name_, float node_radius_, std::vector<std::unique_ptr<NodeCollision>>& vec)
        {
            NodeCollision* node_collision;
            node_collision = new NodeCollision;
            node_collision->bone_name = bone_name_;
            node_collision->node_radius = node_radius_;

            vec.emplace_back(node_collision);
        };

        /*------------------ 喰らい当たり判定のセット --------------*/
        PutInCollisions("J_head", 1.0f, hit_collisions);

        /*--------------- 両腕攻撃の当たり判定配列のセット ------------*/
        PutInCollisions("J_leg_A_01_L", 0.2f, arm_attack_collisions);
        PutInCollisions("J_leg_A_02_L", 0.2f, arm_attack_collisions);
        PutInCollisions("J_leg_A_03_L", 0.2f, arm_attack_collisions);
        PutInCollisions("J_leg_A_01_R", 0.2f, arm_attack_collisions);
        PutInCollisions("J_leg_A_02_R", 0.2f, arm_attack_collisions);
        PutInCollisions("J_leg_A_03_R", 0.2f, arm_attack_collisions);

    }

};

