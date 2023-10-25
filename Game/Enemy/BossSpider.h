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
        max_health = 500;
        health = max_health;

        attack_power = 10.0f;
        speed_power = 70.0f;
        near_attack_range = 6.0f;
        middle_attack_range = 10.0f;
        scaleFactor = 3.0f;

        // メッシュの名前違うからここで再設定
        meshName = "spider_boss_spider_boss1";

        enemy_type = "BossSpider";
    }


    void BehaviorTreeInitialize() override
    {
        BehaviorTreeInitialize_Level5();
    }

    void SetUpHitCollision() override
    {
        // 暗い当たり判定配列にノードをセット
        auto PutInCollisions = [this](const char* bone_name_, float node_radius_, std::vector<std::unique_ptr<NodeCollision>>& vec)
        {
            NodeCollision* node_collision;
            node_collision = new NodeCollision;
            node_collision->bone_name = bone_name_;
            node_collision->node_radius = node_radius_;

            vec.emplace_back(node_collision);
        };

        /*------------------ 喰らい当たり判定のセット --------------*/
        PutInCollisions("J_head_end", 0.8f, hit_collisions);
        PutInCollisions("J_leg_A_03_L", 0.8f, hit_collisions);
        PutInCollisions("J_leg_A_end_L", 0.5f, hit_collisions);
        PutInCollisions("J_leg_B_03_L", 0.8f, hit_collisions);
        PutInCollisions("J_leg_B_end_L", 0.5f, hit_collisions);
        PutInCollisions("J_leg_C_03_L", 0.8f, hit_collisions);
        PutInCollisions("J_leg_C_end_L", 0.5f, hit_collisions);
        PutInCollisions("J_leg_D_03_L", 0.8f, hit_collisions);
        PutInCollisions("J_leg_D_end_L", 0.5f, hit_collisions);
        PutInCollisions("J_leg_A_03_R", 0.8f, hit_collisions);
        PutInCollisions("J_leg_A_end_R", 0.5f, hit_collisions);
        PutInCollisions("J_leg_B_03_R", 0.8f, hit_collisions);
        PutInCollisions("J_leg_B_end_R", 0.5f, hit_collisions);
        PutInCollisions("J_leg_C_03_R", 0.8f, hit_collisions);
        PutInCollisions("J_leg_C_end_R", 0.5f, hit_collisions);
        PutInCollisions("J_leg_D_03_R", 0.8f, hit_collisions);
        PutInCollisions("J_leg_D_end_R", 0.5f, hit_collisions);
        PutInCollisions("J_lowbody", 1.0f, hit_collisions);
        PutInCollisions("J_lowbody_end", 1.0f, hit_collisions);

        PutInCollisions("subJ_leg_D_03_L", 0.8f, hit_collisions);
        PutInCollisions("subJ_leg_D_03_R", 0.8f, hit_collisions);

        /*--------------- 両腕攻撃の当たり判定配列のセット ------------*/
        PutInCollisions("J_leg_A_01_L", 0.8f, arm_attack_collisions);
        PutInCollisions("J_leg_A_02_L", 0.8f, arm_attack_collisions);
        PutInCollisions("J_leg_A_03_L", 0.8f, arm_attack_collisions);
        PutInCollisions("J_leg_A_end_L", 0.5f, arm_attack_collisions);
        PutInCollisions("J_leg_A_01_R", 0.8f, arm_attack_collisions);
        PutInCollisions("J_leg_A_02_R", 0.8f, arm_attack_collisions);
        PutInCollisions("J_leg_A_03_R", 0.8f, arm_attack_collisions);
        PutInCollisions("J_leg_A_end_R", 0.5f, arm_attack_collisions);

    }
};

