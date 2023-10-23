#include "Enemy.h"

#include <directxmath.h>

#include "imgui.h"
#include "Game/AI/ActionDerived.h"
#include "Game/Scene/SceneGame.h"
#include "Lemur/Graphics/Camera.h"
#include "Lemur/Input/GamePad.h"
#include "Lemur/Input/Input.h"
#include "Game/MathHelper.h"
#include "Game/AI/JudgmentDerived.h"
#include "Game/AI/NodeBase.h"
#include "Game/Manager/EnemyManager.h"
#include "Game/StateMachine/StateMachine.h"
#include "Lemur/Collision/Collision.h"

void EnemyGraphicsComponent::Initialize(GameObject* gameobj)
{
    Enemy* enemy = dynamic_cast<Enemy*> (gameobj);
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    enemy->damage_spr = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\number.png");

    if (enemy->enemy_type == "BossSpider")
    {
        enemy->SetModel(ResourceManager::Instance().LoadModelResource(graphics.GetDevice(), ".\\resources\\Enemy\\spiderboss_v004.fbx"));
    }
    else
    {
        enemy->SetModel(ResourceManager::Instance().LoadModelResource(graphics.GetDevice(), ".\\resources\\Enemy\\spider_v006.fbx"));
    }

    // �V�F�[�_�[
    // �J���[
    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_small\\spider_small_color.png", enemy->spider_small_color.GetAddressOf(), graphics.GetTexture2D());
    
    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_middle\\spider_middle_A_Color.png", enemy->spider_middle_color_A.GetAddressOf(), graphics.GetTexture2D());
    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_middle\\spider_middle_B_Color.png", enemy->spider_middle_color_B.GetAddressOf(), graphics.GetTexture2D());
    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_middle\\spider_middle_C_Color.png", enemy->spider_middle_color_C.GetAddressOf(), graphics.GetTexture2D());
    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_middle\\spider_middle_D_Color.png", enemy->spider_middle_color_D.GetAddressOf(), graphics.GetTexture2D());
    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_middle\\spider_middle_E_Color.png", enemy->spider_middle_color_E.GetAddressOf(), graphics.GetTexture2D());
    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_middle\\spider_middle_F_Color.png", enemy->spider_middle_color_F.GetAddressOf(), graphics.GetTexture2D());
    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_middle\\spider_middle_G_Color.png", enemy->spider_middle_color_G.GetAddressOf(), graphics.GetTexture2D());
    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_middle\\spider_middle_H_Color.png", enemy->spider_middle_color_H.GetAddressOf(), graphics.GetTexture2D());
    
    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_boss\\spider_boss_color.png", enemy->spider_boss_color.GetAddressOf(), graphics.GetTexture2D());

    // �m�[�}��
    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_small\\spider_small_Normal.png", enemy->spider_small_normal.GetAddressOf(), graphics.GetTexture2D());

    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_middle\\spider_middle_Normal.png", enemy->spider_middle_normal.GetAddressOf(), graphics.GetTexture2D());
   
    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_boss\\spider_boss_Normal.png", enemy->spider_boss_normal.GetAddressOf(), graphics.GetTexture2D());

    // ���t�l�X
    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_small\\spider_small_Roughness.png", enemy->spider_small_roughness.GetAddressOf(), graphics.GetTexture2D());

    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_middle\\spider_smallmiddle_Roughness.png", enemy->spider_middle_roughness.GetAddressOf(), graphics.GetTexture2D());

    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_boss\\spider_boss_Roughness.png", enemy->spider_boss_roughness.GetAddressOf(), graphics.GetTexture2D());

    // ���^���l�X
    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_small\\spider_small_Metallic.png", enemy->spider_small_metalness.GetAddressOf(), graphics.GetTexture2D());

    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_middle\\spider_middle_Metallic.png", enemy->spider_middle_metalness.GetAddressOf(), graphics.GetTexture2D());

    load_texture_from_file(graphics.GetDevice(), L".\\resources\\Enemy\\spider_boss\\spider_boss_Metallic.png", enemy->spider_boss_metalness.GetAddressOf(), graphics.GetTexture2D());



    //switch (enemy->enemy_type)
    //{
    //case Enemy::SmallSpider:
    //    enemy->SetModel(ResourceManager::Instance().LoadModelResource(graphics.GetDevice(), ".\\resources\\Enemy\\spider_v003.fbx"));
    //    break;
    //case Enemy::BossSpider:
    //    break;
    //}
}

void EnemyGraphicsComponent::Update(GameObject* gameobj)
{
    Enemy* demoPlayer = dynamic_cast<Enemy*> (gameobj);


}

void EnemyGraphicsComponent::Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader)
{
    Enemy* enemy = dynamic_cast<Enemy*> (gameobj);
    
    enemy->Render(elapsedTime, replaced_pixel_shader);

    enemy->DebugImgui();
}


void Enemy::DrawDebugPrimitive()
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

void Enemy::BehaviorTreeInitialize_Level1()
{
    behaviorData = new BehaviorData();
    ai_tree = new BehaviorTree(this);

    ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
    {
#if 0 �f�o�b�O�p
        //// �ҋ@(�f�o�b�O�p)
        //ai_tree->AddNode("Root", "Idle(debug)", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
        // ���r�U��(�f�o�b�O�p)
        ai_tree->AddNode("Root", "TwinArmsAttack(debug)", 3, BehaviorTree::SelectRule::Non, nullptr, new TwinArmsAttackAction(this));
        // �����킹(�f�o�b�O�p)
        ai_tree->AddNode("Root", "Turn(debug)", 2, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
#else
        // ��퓬
        ai_tree->AddNode("Root", "NonBattle", 3, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
        {
            // �p�j
            ai_tree->AddNode("NonBattle", "Wander", 0, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
            // �ҋ@
            ai_tree->AddNode("NonBattle", "Idle", 1, BehaviorTree::SelectRule::Non, nullptr, new TuraAction(this));
        }
        // �퓬
        ai_tree->AddNode("Root", "Battle", 2, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), nullptr);
        {
            // �����킹
            ai_tree->AddNode("Battle", "Turn", 0, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
            // �ߐڐ퓬
            ai_tree->AddNode("Battle", "Near", 1, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
            {
                // ��т��U��
                ai_tree->AddNode("Near", "JumpAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new JumpAttackAction(this));
                //�ҋ@
                ai_tree->AddNode("Near", "Idle", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
            }
            // �������퓬
            ai_tree->AddNode("Battle", "Middle", 2, BehaviorTree::SelectRule::Random, new MiddleJudgment(this), nullptr);
            {
                // �ǐ�
                ai_tree->AddNode("Middle", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
            }
            // �������퓬
            ai_tree->AddNode("Battle", "Far", 3, BehaviorTree::SelectRule::Random, nullptr, nullptr);
            {
                // �ǐ�
                ai_tree->AddNode("Far", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
            }
        }
#endif
        // ���S
        ai_tree->AddNode("Root", "Death", 0, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));

    }

}

void Enemy::BehaviorTreeInitialize_Level2()
{
    behaviorData = new BehaviorData();
    ai_tree = new BehaviorTree(this);

    ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
    {
#if 0 �f�o�b�O�p
        //// �ҋ@(�f�o�b�O�p)
        //ai_tree->AddNode("Root", "Idle(debug)", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
        // ���r�U��(�f�o�b�O�p)
        ai_tree->AddNode("Root", "TwinArmsAttack(debug)", 3, BehaviorTree::SelectRule::Non, nullptr, new TwinArmsAttackAction(this));
        // �����킹(�f�o�b�O�p)
        ai_tree->AddNode("Root", "Turn(debug)", 2, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
#else
        // ��퓬
        ai_tree->AddNode("Root", "NonBattle", 3, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
        {
            // �p�j
            ai_tree->AddNode("NonBattle", "Wander", 0, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
            // �ҋ@
            ai_tree->AddNode("NonBattle", "Idle", 1, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
        }
        // �퓬
        ai_tree->AddNode("Root", "Battle", 2, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), nullptr);
        {
            // �����킹
            ai_tree->AddNode("Battle", "Turn", 0, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
            // �ߐڐ퓬
            ai_tree->AddNode("Battle", "Near", 1, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
            {
                // �ҋ@or�����킹�����r�U���̃R���{
                ai_tree->AddNode("Near", "Combo_B", 0, BehaviorTree::SelectRule::Sequence, nullptr, nullptr);
                {
                    // �ҋ@�������킹
                    ai_tree->AddNode("Combo_B", "Idle_or_Turn", 0, BehaviorTree::SelectRule::Random, nullptr, nullptr);
                    {
                        ai_tree->AddNode("Idle_or_Turn", "Idle", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
                        ai_tree->AddNode("Idle_or_Turn", "Turn", 0, BehaviorTree::SelectRule::Non, nullptr, new TuraAction(this));
                    }
                    //���r�U��
                    ai_tree->AddNode("Combo_B", "TwinArmsAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new TwinArmsAttackAction(this));
                }
                //���r�U��
                ai_tree->AddNode("Near", "TwinArmsAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new TwinArmsAttackAction(this));
                // �o�b�N�X�e�b�v
                ai_tree->AddNode("Near", "BackStep", 0, BehaviorTree::SelectRule::Non, nullptr, new BackStepAction(this));
            }
            // �������퓬
            ai_tree->AddNode("Battle", "Middle", 2, BehaviorTree::SelectRule::Random, new MiddleJudgment(this), nullptr);
            {
                // �ǐ�
                ai_tree->AddNode("Middle", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
            }
            // �������퓬
            ai_tree->AddNode("Battle", "Far", 3, BehaviorTree::SelectRule::Random, nullptr, nullptr);
            {
                // �ǐ�
                ai_tree->AddNode("Far", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
            }
        }
#endif
        // ���S
        ai_tree->AddNode("Root", "Death", 0, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));

    }

}

void Enemy::BehaviorTreeInitialize_Level3()
{
    behaviorData = new BehaviorData();
    ai_tree = new BehaviorTree(this);

    ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
    {
#if 0 �f�o�b�O�p
        //// �ҋ@(�f�o�b�O�p)
        //ai_tree->AddNode("Root", "Idle(debug)", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
        // ���r�U��(�f�o�b�O�p)
        ai_tree->AddNode("Root", "TwinArmsAttack(debug)", 3, BehaviorTree::SelectRule::Non, nullptr, new TwinArmsAttackAction(this));
        // �����킹(�f�o�b�O�p)
        ai_tree->AddNode("Root", "Turn(debug)", 2, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
#else
        // ��퓬
        ai_tree->AddNode("Root", "NonBattle", 3, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
        {
            // �p�j
            ai_tree->AddNode("NonBattle", "Wander", 0, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
            // �ҋ@
            ai_tree->AddNode("NonBattle", "Idle", 1, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
        }
        // �퓬
        ai_tree->AddNode("Root", "Battle", 2, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), nullptr);
        {
            // �����킹
            ai_tree->AddNode("Battle", "Turn", 0, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
            // �ߐڐ퓬
            ai_tree->AddNode("Battle", "Near", 1, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
            {
                // ��т��U��
                ai_tree->AddNode("Near", "JumpAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new JumpAttackAction(this));
            
                // �ҋ@or�����킹����т��U���̃R���{
                ai_tree->AddNode("Near", "Combo_B", 0, BehaviorTree::SelectRule::Sequence, nullptr, nullptr);
                {
                    // �ҋ@�������킹
                    ai_tree->AddNode("Combo_B", "Idle_or_Turn", 0, BehaviorTree::SelectRule::Random, nullptr, nullptr);
                    {
                        ai_tree->AddNode("Idle_or_Turn", "Idle", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
                        ai_tree->AddNode("Idle_or_Turn", "Turn", 0, BehaviorTree::SelectRule::Non, nullptr, new TuraAction(this));
                    }
                    //��т��U��
                    ai_tree->AddNode("Combo_B", "JumpAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new JumpAttackAction(this));
                }
                // �o�b�N�X�e�b�v
                ai_tree->AddNode("Near", "BackStep", 0, BehaviorTree::SelectRule::Non, nullptr, new BackStepAction(this));
            }
            // �������퓬
            ai_tree->AddNode("Battle", "Middle", 2, BehaviorTree::SelectRule::Random, new MiddleJudgment(this), nullptr);
            {
                // �Ńu���X
                ai_tree->AddNode("Middle", "PoisonBreath", 0, BehaviorTree::SelectRule::Non, nullptr, new PoisonAttackAction(this));

            }
            // �������퓬
            ai_tree->AddNode("Battle", "Far", 3, BehaviorTree::SelectRule::Random, nullptr, nullptr);
            {
                // �ǐ�
                ai_tree->AddNode("Far", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
                // �Ńu���X
                ai_tree->AddNode("Far", "PoisonBreath", 0, BehaviorTree::SelectRule::Non, nullptr, new PoisonAttackAction(this));
            }
        }
#endif
        // ���S
        ai_tree->AddNode("Root", "Death", 0, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));

    }

}

void Enemy::BehaviorTreeInitialize_Level4()
{
    behaviorData = new BehaviorData();
    ai_tree = new BehaviorTree(this);

    ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
    {
#if 0 �f�o�b�O�p
        //// �ҋ@(�f�o�b�O�p)
        //ai_tree->AddNode("Root", "Idle(debug)", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
        // ���r�U��(�f�o�b�O�p)
        ai_tree->AddNode("Root", "TwinArmsAttack(debug)", 3, BehaviorTree::SelectRule::Non, nullptr, new TwinArmsAttackAction(this));
        // �����킹(�f�o�b�O�p)
        ai_tree->AddNode("Root", "Turn(debug)", 2, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
#else
        // ��퓬
        ai_tree->AddNode("Root", "NonBattle", 3, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
        {
            // �p�j
            ai_tree->AddNode("NonBattle", "Wander", 0, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
            // �ҋ@
            ai_tree->AddNode("NonBattle", "Idle", 1, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
        }
        // �퓬
        ai_tree->AddNode("Root", "Battle", 2, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), nullptr);
        {
            // �����킹
            ai_tree->AddNode("Battle", "Turn", 0, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
            // �ߐڐ퓬
            ai_tree->AddNode("Battle", "Near", 1, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
            {
                // ��т�����i�U���̃R���{
                ai_tree->AddNode("Near", "Combo_A", 0, BehaviorTree::SelectRule::Sequence, nullptr, nullptr);
                {
                    ai_tree->AddNode("Combo_A", "JumpAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new JumpAttackAction(this));
                }
                // �ҋ@or�����킹����i�U��or���r�U��or��т��U���̃R���{
                ai_tree->AddNode("Near", "Combo_B", 0, BehaviorTree::SelectRule::Sequence, nullptr, nullptr);
                {
                    // �ҋ@�������킹
                    ai_tree->AddNode("Combo_B", "Idle_or_Turn", 0, BehaviorTree::SelectRule::Random, nullptr, nullptr);
                    {
                        ai_tree->AddNode("Idle_or_Turn", "Idle", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
                        ai_tree->AddNode("Idle_or_Turn", "Turn", 0, BehaviorTree::SelectRule::Non, nullptr, new TuraAction(this));
                    }
                    // ��i�U�������r�U������т��U��
                    ai_tree->AddNode("Combo_B", "DA_or_TA_or_JA", 0, BehaviorTree::SelectRule::Random, nullptr, nullptr);
                    {
                        ai_tree->AddNode("DA_or_TA_or_JA", "DoubleAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new DoubleAttackAction(this));
                        ai_tree->AddNode("DA_or_TA_or_JA", "TwinArmsAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new TwinArmsAttackAction(this));
                        ai_tree->AddNode("DA_or_TA_or_JA", "JumpAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new JumpAttackAction(this));
                    }
                }
                // �o�b�N�X�e�b�v
                ai_tree->AddNode("Near", "BackStep", 0, BehaviorTree::SelectRule::Non, nullptr, new BackStepAction(this));
                // �U��
                ai_tree->AddNode("Near", "Attack", 1, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
                {
                    ai_tree->AddNode("Attack", "NearAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new NearAttackAction(this));
                }
            }
            // �������퓬
            ai_tree->AddNode("Battle", "Middle", 2, BehaviorTree::SelectRule::Random, new MiddleJudgment(this), nullptr);
            {
                // �ǐ�
                ai_tree->AddNode("Middle", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
                // �Ńu���X
                ai_tree->AddNode("Middle", "PoisonBreath", 0, BehaviorTree::SelectRule::Non, nullptr, new PoisonAttackAction(this));

            }
            // �������퓬
            ai_tree->AddNode("Battle", "Far", 3, BehaviorTree::SelectRule::Random, nullptr, nullptr);
            {
                // �ːi�U��
                ai_tree->AddNode("Far", "Rush", 0, BehaviorTree::SelectRule::Non, nullptr, new RushAttackAction(this));
                // �Ńu���X
                ai_tree->AddNode("Far", "PoisonBreath", 0, BehaviorTree::SelectRule::Non, nullptr, new PoisonAttackAction(this));
            }
        }
#endif
        // ���S
        ai_tree->AddNode("Root", "Death", 0, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));

    }

}

void Enemy::BehaviorTreeInitialize_Level5()
{
    behaviorData = new BehaviorData();
    ai_tree = new BehaviorTree(this);

    ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
    {
#if 0 �f�o�b�O�p
        //// �ҋ@(�f�o�b�O�p)
        //ai_tree->AddNode("Root", "Idle(debug)", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
        //// �o�b�N�X�e�b�v
        //ai_tree->AddNode("Root", "BackStep", 0, BehaviorTree::SelectRule::Non, nullptr, new BackStepAction(this));
        // �������Ȃ�
        ai_tree->AddNode("Root", "null", 0, BehaviorTree::SelectRule::Non, nullptr, nullptr);

        //// ���r�U��(�f�o�b�O�p)
        //ai_tree->AddNode("Root", "TwinArmsAttack(debug)", 3, BehaviorTree::SelectRule::Non, nullptr, new TwinArmsAttackAction(this));
        //// �����킹(�f�o�b�O�p)
        //ai_tree->AddNode("Root", "Turn(debug)", 2, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
#else
        // ��퓬
        ai_tree->AddNode("Root", "NonBattle", 3, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
        {
            // �p�j
            ai_tree->AddNode("NonBattle", "Wander", 0, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
            // �ҋ@
            ai_tree->AddNode("NonBattle", "Idle", 1, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
        }
        // �퓬
        ai_tree->AddNode("Root", "Battle", 2, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), nullptr);
        {
            // �����킹
            ai_tree->AddNode("Battle", "Turn", 0, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
            // �ߐڐ퓬
            ai_tree->AddNode("Battle", "Near", 1, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
            {
                // ��т�����i�U���̃R���{
                ai_tree->AddNode("Near", "Combo_A", 0, BehaviorTree::SelectRule::Sequence, nullptr, nullptr);
                {
                    ai_tree->AddNode("Combo_A", "JumpAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new JumpAttackAction(this));
                }
                // �ҋ@or�����킹����i�U��or���r�U��or��т��U���̃R���{
                ai_tree->AddNode("Near", "Combo_B", 0, BehaviorTree::SelectRule::Sequence, nullptr, nullptr);
                {
                    // �ҋ@�������킹
                    ai_tree->AddNode("Combo_B", "Idle_or_Turn", 0, BehaviorTree::SelectRule::Random, nullptr, nullptr);
                    {
                        ai_tree->AddNode("Idle_or_Turn", "Idle", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
                        ai_tree->AddNode("Idle_or_Turn", "Turn", 0, BehaviorTree::SelectRule::Non, nullptr, new TuraAction(this));
                    }
                    // ��i�U�������r�U������т��U��
                    ai_tree->AddNode("Combo_B", "DA_or_TA_or_JA", 0, BehaviorTree::SelectRule::Random, nullptr, nullptr);
                    {
                        ai_tree->AddNode("DA_or_TA_or_JA", "DoubleAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new DoubleAttackAction(this));
                        ai_tree->AddNode("DA_or_TA_or_JA", "TwinArmsAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new TwinArmsAttackAction(this));
                        ai_tree->AddNode("DA_or_TA_or_JA", "JumpAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new JumpAttackAction(this));
                    }
                }
                // �o�b�N�X�e�b�v
                ai_tree->AddNode("Near", "BackStep", 0, BehaviorTree::SelectRule::Non, nullptr, new BackStepAction(this));
                // �U��
                ai_tree->AddNode("Near", "Attack", 1, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
                {
                    ai_tree->AddNode("Attack", "NearAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new NearAttackAction(this));
                }
            }
            // �������퓬
            ai_tree->AddNode("Battle", "Middle", 2, BehaviorTree::SelectRule::Random, new MiddleJudgment(this), nullptr);
            {
                // �ǐ�
                ai_tree->AddNode("Middle", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
                // �Ńu���X
                ai_tree->AddNode("Middle", "PoisonBreath", 0, BehaviorTree::SelectRule::Non, nullptr, new PoisonAttackAction(this));

            }
            // �������퓬
            ai_tree->AddNode("Battle", "Far", 3, BehaviorTree::SelectRule::Random, nullptr, nullptr);
            {
                // �ːi�U��
                ai_tree->AddNode("Far", "Rush", 0, BehaviorTree::SelectRule::Non, nullptr, new RushAttackAction(this));
                // �Ńu���X
                ai_tree->AddNode("Far", "PoisonBreath", 0, BehaviorTree::SelectRule::Non, nullptr, new PoisonAttackAction(this));
            }
        }
#endif
        // ����
        ai_tree->AddNode("Root", "Fear", 1, BehaviorTree::SelectRule::Non, new FearJudgment(this), new FearAction(this));
        // ���S
        ai_tree->AddNode("Root", "Death", 0, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));

    }

}

void Enemy::BehaviorTreeUpdate()
{
    // ���ݎ��s����Ă���m�[�h���������
    if (activeNode == nullptr)
    {
        // ���Ɏ��s����m�[�h�𐄘_����B
        activeNode = ai_tree->ActiveNodeInference(behaviorData);
    }
    // ���ݎ��s����m�[�h�������
    if (activeNode != nullptr)
    {
        // �r�w�C�r�A�c���[����m�[�h�����s�B
        activeNode = ai_tree->Run(activeNode, behaviorData, high_resolution_timer::Instance().time_interval());
    }

}

// todo ���ꃂ�W���[��������Nero�ɓ����B
bool Enemy::DistanceJudge(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target_pos, float judge_range)
{
    //float vx = target_pos.x - pos.x;
    //float vz = target_pos.z - pos.z;

    //// �^�[�Q�b�g�ʒu�܂ł̋���(���)
    //float dist_sq = ((vx * vx) + (vz * vz));
    //judge_range *= 2.0f;

    float dist = Length(pos, target_pos);

    if (dist < judge_range)
    {
        return true;
    }

    return false;
}

bool Enemy::SearchPlayer(float found_distance, float found_range)
{
    // �v���C���[�̈ʒu
    DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();

    // x������
    float vec_x = player_pos.x - position.x;
    // z������
    float vec_z = player_pos.z - position.z;
    // ���v����(���)
    float length_sq = (vec_x * vec_x) + (vec_z * vec_z);

    // �������͈͈ȓ��Ȃ�
    if (length_sq < found_distance * found_distance)
    {
        //���͐��ʔ���

        // �O����X�x�N�g��
        float front_x = sinf(rotation.y);
        // �O����Z�x�N�g��
        float front_z = cosf(rotation.y);

        // �����O��
        float length = sqrtf(length_sq);
        // ���K��
        vec_x /= length;
        vec_z /= length;

        // ����
        float dot = (front_x * vec_x) + (front_z * vec_z);
        // ���ʂɋ�����
        if (dot > found_range)
        {
            return true;
        }
    }
    return false;
}


void Enemy::Move_to_Target(float elapsedTime, float move_speed_rate, float turn_speed_rate)
{
    float vx = target_position.x - position.x;
    float vz = target_position.z - position.z;
    float dist = sqrtf((vx * vx) + (vz * vz));

    // ���K��
    vx /= dist;
    vz /= dist;

    // todo ������ӂ����̓e�L�g�[
    Move(vx, vz, walk_speed *speed_power* move_speed_rate);
    Turn(vx, vz, 5.0f);

}

void Enemy::CollisionNodeVsPlayer(const char* mesh_name, const char* bone_name, float node_radius)
{
    Player* player = CharacterManager::Instance().GetPlayer();

    // ���G���Ȃ画�肵�Ȃ�
    if (player->invincible)
        return;

    // �m�[�h�ʒu�擾
    DirectX::XMFLOAT3 nodePosition = Model->joint_position(mesh_name, bone_name, &keyframe, world);

    if (Collision::IntersectSphereVsCylinder(
        nodePosition, node_radius,
        player->GetPosition(), player->GetRadius(), player->GetHeight())
        )
    {
        if (player->CounterJudge(nodePosition)) {}
        else if (player->ApplyDamage(attack_power * player->GetDamageCorrection()))
        {
            // �����X�L�������Ă��狭���I��HP0�ɂ���
            if (player->HaveSkill("Tofu"))
            {
                player->health = 0;
                player->death = true;
                return;
            }

            player->GetStateMachine()->SetNextState(player->Fear_State);
        }
        return;
    }
}

void Enemy::SetRandomTargetPosition()
{
    // territory_range�̑傫���̉~�Ń����_���Ȓl���Ƃ�B
    float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
    float range = Mathf::RandomRange(0.0f, territory_range);

    target_position.x = territory_origin.x + sinf(theta) * range;
    target_position.y = territory_origin.y;
    target_position.z = territory_origin.z + cosf(theta) * range;

}

void Enemy::DamageRender(const float damage)
{
    // �r���[�|�[�g
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    ID3D11DeviceContext* dc = Lemur::Graphics::Graphics::Instance().GetDeviceContext();
    dc->RSGetViewports(&numViewports, &viewport);

    // �ϊ��s�� (�v�Z���₷���悤�ɍs��ɕϊ�)
    DirectX::XMMATRIX V = Camera::Instance().GetViewMatrix();
    DirectX::XMMATRIX P = Camera::Instance().GetProjectionMatrix();
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    // �x�N�g���ϊ�
    DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&position);

    DirectX::XMVECTOR ScreenPosition;

    ScreenPosition = DirectX::XMVector3Project(Pos,
        viewport.TopLeftX,
        viewport.TopLeftY,
        viewport.Width,
        viewport.Height,
        viewport.MinDepth,
        viewport.MaxDepth,
        P,
        V,
        World
    );
    DirectX::XMFLOAT3 screenPosition;
    DirectX::XMStoreFloat3(&screenPosition, ScreenPosition);
    damage_spr->textout(Lemur::Graphics::Graphics::Instance().GetDeviceContext(), std::to_string(damage), screenPosition.x, screenPosition.y, 50, 50, 1, 1, 1, 1);

}

void Enemy::DebugImgui()
{
    ImGui::Begin("Enemy");
    ImGui::Text(enemy_type.c_str());
    if (ImGui::TreeNode("Transform"))
    {
        ImGui::DragFloat3("position", &position.x);
        ImGui::SliderAngle("rotation", &rotation.y);
        ImGui::DragFloat("scale_factor", &scaleFactor);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("AI"))
    {
        if (!activeNode)
            ImGui::Text("null");
        else
            ImGui::Text(activeNode->GetName().c_str());

        ImGui::DragFloat("near_attack_range", &near_attack_range);
        ImGui::DragFloat("middle_attack_range", &middle_attack_range);
        ImGui::DragFloat3("target_position", &target_position.x);

        DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();
        float enemy_to_player = Length(position, player_pos);
        ImGui::DragFloat("enemy_to_player", &enemy_to_player);

        float enemy_to_target = Length(target_position, position);
        ImGui::DragFloat("enemy_to_target", &enemy_to_target);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Attack"))
    {
        ImGui::DragFloat("near_attack_range", &near_attack_range);
        ImGui::Checkbox("attack_collision_flag", &attack_collision_flag);
        ImGui::InputFloat("attack_collision_range", &attack_collision_range);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("parameter"))
    {
        ImGui::DragInt("MaxHealth", &max_health);
        ImGui::DragInt("health", &health);
        if (health <= 0)
        {
            health = 0;
            death = true;
        }
        ImGui::DragFloat("Walk_speed", &walk_speed);
        ImGui::DragFloat("attack_power", &attack_power);
        ImGui::DragFloat("defense_power", &defense_power);
        ImGui::DragFloat("speed_power", &speed_power);
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("HitCollisions"))
    {
        for (auto& collision : hit_collisions)
        {
            ImGui::Text(collision->bone_name.c_str());
            ImGui::DragFloat("node_radius", &collision->node_radius);
        }
        ImGui::TreePop();
    }
    ImGui::Checkbox("is_hit", &is_hit);

    ImGui::End();

}

void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}

void Enemy::SetUpHitCollisionNormalSize()
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
    PutInCollisions("J_head", 1.0f, hit_collisions);
    //PutInCollisions("J_head_end", 0.8f, hit_collisions);

    //PutInCollisions("J_leg_A_02_L", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_A_03_L", 0.8f, hit_collisions);
    ////PutInCollisions("J_leg_A_end_L", 0.5f, hit_collisions);

    //PutInCollisions("J_leg_B_02_L", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_B_03_L", 0.8f, hit_collisions);
    ////PutInCollisions("J_leg_B_end_L", 0.5f, hit_collisions);

    //PutInCollisions("J_leg_C_02_L", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_C_03_L", 0.8f, hit_collisions);
    ////PutInCollisions("J_leg_C_end_L", 0.5f, hit_collisions);

    //PutInCollisions("J_leg_D_02_L", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_D_03_L", 0.8f, hit_collisions);
    ////PutInCollisions("J_leg_D_end_L", 0.5f, hit_collisions);

    //PutInCollisions("J_leg_A_02_R", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_A_03_R", 0.8f, hit_collisions);
    ////PutInCollisions("J_leg_A_end_R", 0.5f, hit_collisions);

    //PutInCollisions("J_leg_B_02_R", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_B_03_R", 0.8f, hit_collisions);
    ////PutInCollisions("J_leg_B_end_R", 0.5f, hit_collisions);

    //PutInCollisions("J_leg_C_02_R", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_C_03_R", 0.8f, hit_collisions);
    ////PutInCollisions("J_leg_C_end_R", 0.5f, hit_collisions);

    //PutInCollisions("J_leg_D_02_R", 0.8f, hit_collisions);
    //PutInCollisions("J_leg_D_03_R", 0.8f, hit_collisions);
    ////PutInCollisions("J_leg_D_end_R", 0.5f, hit_collisions);

    //PutInCollisions("J_lowbody", 1.0f, hit_collisions);
    ////PutInCollisions("J_lowbody_end", 1.0f, hit_collisions);

    /*--------------- ���r�U���̓����蔻��z��̃Z�b�g ------------*/
    PutInCollisions("J_leg_A_01_L", 0.2f, arm_attack_collisions);
    PutInCollisions("J_leg_A_02_L", 0.2f, arm_attack_collisions);
    PutInCollisions("J_leg_A_03_L", 0.2f, arm_attack_collisions);
    //PutInCollisions("J_leg_A_end_L", 0.5f, arm_attack_collisions);
    PutInCollisions("J_leg_A_01_R", 0.2f, arm_attack_collisions);
    PutInCollisions("J_leg_A_02_R", 0.2f, arm_attack_collisions);
    PutInCollisions("J_leg_A_03_R", 0.2f, arm_attack_collisions);
    //PutInCollisions("J_leg_A_end_R", 0.5f, arm_attack_collisions);

}


// ���͏���
void EnemyInputComponent::Update(GameObject* gameobj, float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();

    float lx = gamePad.GetAxisLX();
    float ly = gamePad.GetAxisLY();

}

DirectX::XMFLOAT3 EnemyInputComponent::GetMoveVec(float input_x, float input_y)
{

    // �J���������ƃX�e�C�b�N�̓��͒l�ɂ���Đi�s�������v�Z����B

    /*--------------- �J���������̌v�Z ---------------*/

    Camera& camera = Camera::Instance();

    DirectX::XMMATRIX V = camera.GetViewMatrix();
    DirectX::XMMATRIX W = DirectX::XMMatrixInverse(nullptr, V);
    DirectX::XMFLOAT4X4 w;
    DirectX::XMStoreFloat4x4(&w, W);

    const DirectX::XMFLOAT3& cameraRight = { w._11,w._12,w._13 };
    const DirectX::XMFLOAT3& cameraFront = { w._31,w._32,w._33 };

    // �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���B

    // �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ��B
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    const float cameraRightLength = sqrtf(
        (cameraRight.x * cameraRight.x) + cameraRight.z * cameraRight.z
    );
    if (cameraRightLength > 0.0f)
    {
        // �P�ʃx�N�g����(���K��)
        cameraRightX = cameraRightX / cameraRightLength;
        cameraRightZ = cameraRightZ / cameraRightLength;
    }

    //�J�����O�����̃x�N�g����XZ�P�ʃx�N�g���ɕϊ��B
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(
        (cameraFront.x * cameraFront.x) + cameraFront.z * cameraFront.z
    );
    if (cameraFrontLength > 0.0f)
    {
        //�P�ʃx�N�g����(���K��)
        cameraFrontX = cameraFrontX / cameraFrontLength;
        cameraFrontZ = cameraFrontZ / cameraFrontLength;
    }

    // �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
    //�X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
    //�i�s�x�N�g�����v�Z����B
    DirectX::XMFLOAT3 vec{};
    vec.x = cameraFrontX * sinf(input_y) + cameraRightX * sinf(input_x);
    vec.z = cameraFrontZ * sinf(input_y) + cameraRightZ * sinf(input_x);
    //y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;


    return vec;

}

void EnemyPhysicsComponent::Initialize(GameObject* gameobj)
{
    Enemy* enemy = dynamic_cast<Enemy*> (gameobj);

    // todo �����Ńe���g���[��ݒ�(�Ƃ肠�������͌��_)
    
    enemy->BehaviorTreeInitialize();
    enemy->SetUpHitCollision();
}

void EnemyPhysicsComponent::Update(GameObject* gameobj, float elapsedTime)
{
    Enemy* enemy = dynamic_cast<Enemy*> (gameobj);

    enemy->BehaviorTreeUpdate();

    enemy->UpdateVelocity(elapsedTime);
}
