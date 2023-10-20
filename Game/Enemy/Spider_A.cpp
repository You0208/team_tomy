#include "Spider_A.h"

void Spider_A::DrawDebugPrimitive()
{
    DebugRenderer* debug_renderer = Lemur::Graphics::Graphics::Instance().GetDebugRenderer();
    //DirectX::XMFLOAT3 position = Model->joint_position(meshName.c_str(), "J_root", &keyframe, world);
    //debug_renderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f));

    if (attack_collision_flag)
    {
        for (auto& collision : arm_attack_collisions)
        {
            DirectX::XMFLOAT3 node_coll_pos = Model->joint_position(meshName.c_str(), collision->bone_name.c_str(), &keyframe, world);
            debug_renderer->DrawSphere(node_coll_pos, collision->node_radius, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f));
        }
    }
    else
    {
        for (auto& collision : hit_collisions)
        {
            DirectX::XMFLOAT3 node_coll_pos = Model->joint_position(meshName.c_str(), collision->bone_name.c_str(), &keyframe, world);
            debug_renderer->DrawSphere(node_coll_pos, collision->node_radius, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f));
        }
    }

}

void Spider_A::BehaviorTreeInitialize()
{
    BehaviorTreeInitialize_Level1();
}

void Spider_A::SetUpHitCollision()
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
    // todo �Ȃ񂩃m�[�h��������
    /*------------------ ��炢�����蔻��̃Z�b�g --------------*/
    PutInCollisions("J_head", 0.8f, hit_collisions);
    //PutInCollisions("J_head_end", 0.8f, hit_collisions);

    PutInCollisions("J_leg_A_02_L", 0.8f, hit_collisions);
    PutInCollisions("J_leg_A_03_L", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_A_end_L", 0.5f, hit_collisions);

    PutInCollisions("J_leg_B_02_L", 0.8f, hit_collisions);
    PutInCollisions("J_leg_B_03_L", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_B_end_L", 0.5f, hit_collisions);

    PutInCollisions("J_leg_C_02_L", 0.8f, hit_collisions);
    PutInCollisions("J_leg_C_03_L", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_C_end_L", 0.5f, hit_collisions);

    PutInCollisions("J_leg_D_02_L", 0.8f, hit_collisions);
    PutInCollisions("J_leg_D_03_L", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_D_end_L", 0.5f, hit_collisions);

    PutInCollisions("J_leg_A_02_R", 0.8f, hit_collisions);
    PutInCollisions("J_leg_A_03_R", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_A_end_R", 0.5f, hit_collisions);

    PutInCollisions("J_leg_B_02_R", 0.8f, hit_collisions);
    PutInCollisions("J_leg_B_03_R", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_B_end_R", 0.5f, hit_collisions);

    PutInCollisions("J_leg_C_02_R", 0.8f, hit_collisions);
    PutInCollisions("J_leg_C_03_R", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_C_end_R", 0.5f, hit_collisions);

    PutInCollisions("J_leg_D_02_R", 0.8f, hit_collisions);
    PutInCollisions("J_leg_D_03_R", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_D_end_R", 0.5f, hit_collisions);

    PutInCollisions("J_lowbody", 1.0f, hit_collisions);
    //PutInCollisions("J_lowbody_end", 1.0f, hit_collisions);

    /*--------------- ���r�U���̓����蔻��z��̃Z�b�g ------------*/
    PutInCollisions("J_leg_A_01_L", 0.8f, arm_attack_collisions);
    PutInCollisions("J_leg_A_02_L", 0.8f, arm_attack_collisions);
    PutInCollisions("J_leg_A_03_L", 0.8f, arm_attack_collisions);
    //PutInCollisions("J_leg_A_end_L", 0.5f, arm_attack_collisions);
    PutInCollisions("J_leg_A_01_R", 0.8f, arm_attack_collisions);
    PutInCollisions("J_leg_A_02_R", 0.8f, arm_attack_collisions);
    PutInCollisions("J_leg_A_03_R", 0.8f, arm_attack_collisions);
    //PutInCollisions("J_leg_A_end_R", 0.5f, arm_attack_collisions);

}
