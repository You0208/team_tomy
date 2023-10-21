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
        position.z += 150;
        rotation.y = 180;
        max_health = 5000;
        health = max_health;

        attack_power = 50.0f;
        speed_power = 70.0f;
        near_attack_range = 6.0f;
        middle_attack_range = 10.0f;
        scaleFactor = 3.0f;

        // ���b�V���̖��O�Ⴄ���炱���ōĐݒ�
        meshName = "spider_boss_spider_boss";

        enemy_type = "BossSpider";
    }


    void BehaviorTreeInitialize() override
    {
        BehaviorTreeInitialize_Level5();
    }

    void SetUpHitCollision() override
    {
        // �Â������蔻��z��Ƀm�[�h���Z�b�g
        auto PutInCollisions = [this](const char* bone_name_, float node_radius_, std::vector<std::unique_ptr<NodeCollision>>& vec)
        {
            NodeCollision* node_collision;
            node_collision = new NodeCollision;
            node_collision->bone_name = bone_name_;
            node_collision->node_radius = node_radius_;

            vec.emplace_back(node_collision);
        };

        // todo ��炢�̓����蔻��̗ʂ����Ȃ��B���N�Ƀ��O�����Ă��炤
        /*------------------ ��炢�����蔻��̃Z�b�g --------------*/
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

        /*--------------- ���r�U���̓����蔻��z��̃Z�b�g ------------*/
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

