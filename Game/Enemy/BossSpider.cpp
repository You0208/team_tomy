#include "BossSpider.h"

#include "Game/AI/ActionDerived.h"
#include "Game/AI/JudgmentDerived.h"

void BossSpider::DrawDebugPrimitive()
{
    DebugRenderer* debug_renderer = Lemur::Graphics::Graphics::Instance().GetDebugRenderer();
    DirectX::XMFLOAT3 position = Model->joint_position("spider_boss_spider_boss", "J_root", &keyframe, world);
    debug_renderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f));

    if (attack_collision_flag)
    {
        for (auto& collision : arm_attack_collisions)
        {
            DirectX::XMFLOAT3 node_coll_pos = Model->joint_position(meshName.c_str(), collision->bone_name.c_str(), &keyframe, world);
            debug_renderer->DrawSphere(node_coll_pos, collision->node_radius, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f));
        }
    }

    //for(auto& collision:hit_collisions)
    //{
    //    DirectX::XMFLOAT3 node_coll_pos = Model->joint_position(meshName.c_str(), collision->bone_name.c_str(), &keyframe, world);
    //    debug_renderer->DrawSphere(node_coll_pos, collision->node_radius, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f));
    //}
}

void BossSpider::BehaviorTreeInitialize()
{
    behaviorData = new BehaviorData();
    ai_tree = new BehaviorTree(this);

    ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
    {
#if 1 デバッグ用
        //// 待機(デバッグ用)
        //ai_tree->AddNode("Root", "Idle(debug)", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
        // 両腕攻撃(デバッグ用)
        ai_tree->AddNode("Root", "TwinArmsAttack(debug)", 3, BehaviorTree::SelectRule::Non, nullptr, new TwinArmsAttackAction(this));
        // 軸合わせ(デバッグ用)
        ai_tree->AddNode("Root", "Turn(debug)", 2, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
#else
        // 非戦闘
        ai_tree->AddNode("Root", "NonBattle", 3, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
        {
            // 徘徊
            ai_tree->AddNode("NonBattle", "Wander", 0, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
            // 待機
            ai_tree->AddNode("NonBattle", "Idle", 1, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
        }
        // 戦闘
        ai_tree->AddNode("Root", "Battle", 2, BehaviorTree::SelectRule::Priority,new BattleJudgment(this) , nullptr);
        {
            // 軸合わせ
            ai_tree->AddNode("Battle", "Turn", 0, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
            // 近接戦闘
            ai_tree->AddNode("Battle", "Near", 1, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
            {
                // 飛びつき→二段攻撃のコンボ
                ai_tree->AddNode("Near", "Combo_A", 0, BehaviorTree::SelectRule::Sequence, nullptr, nullptr);
                {
                    ai_tree->AddNode("Combo_A", "JumpAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new JumpAttackAction(this));
                }
                // 待機or軸合わせ→二段攻撃or両腕攻撃or飛びつき攻撃のコンボ
                ai_tree->AddNode("Near", "Combo_B", 0, BehaviorTree::SelectRule::Sequence, nullptr, nullptr);
                {
                    // 待機か軸合わせ
                    ai_tree->AddNode("Combo_B", "Idle_or_Turn", 0, BehaviorTree::SelectRule::Random, nullptr, nullptr);
                    {
                        ai_tree->AddNode("Idle_or_Turn", "Idle", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
                        ai_tree->AddNode("Idle_or_Turn", "Turn", 0, BehaviorTree::SelectRule::Non, nullptr, new TuraAction(this));
                    }
                    // 二段攻撃か両腕攻撃か飛びつき攻撃
                    ai_tree->AddNode("Combo_B", "DA_or_TA_or_JA", 0, BehaviorTree::SelectRule::Random, nullptr, nullptr);
                    {
                        ai_tree->AddNode("DA_or_TA_or_JA", "DoubleAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new DoubleAttackAction(this));
                        ai_tree->AddNode("DA_or_TA_or_JA", "TwinArmsAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new TwinArmsAttackAction(this));
                        ai_tree->AddNode("DA_or_TA_or_JA", "JumpAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new JumpAttackAction(this));
                    }
                }
                // バックステップ
                ai_tree->AddNode("Near", "BackStep", 0, BehaviorTree::SelectRule::Non, nullptr, new BackStepAction(this));
                // 攻撃
                ai_tree->AddNode("Near", "Attack", 1, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
                {
                    ai_tree->AddNode("Attack", "NearAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new NearAttackAction(this));
                }
            }
            // 中距離戦闘
            ai_tree->AddNode("Battle", "Middle", 2, BehaviorTree::SelectRule::Random, new MiddleJudgment(this), nullptr);
            {
                // 追跡
                ai_tree->AddNode("Middle", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
                // 毒ブレス
                ai_tree->AddNode("Middle", "PoisonBreath", 0, BehaviorTree::SelectRule::Non, nullptr, new PoisonAttackAction(this));

            }
            // 遠距離戦闘
            ai_tree->AddNode("Battle", "Far", 3, BehaviorTree::SelectRule::Random, nullptr, nullptr);
            {
                // 突進攻撃
                ai_tree->AddNode("Far", "Rush", 0, BehaviorTree::SelectRule::Non, nullptr, new RushAttackAction(this));
                    // 毒ブレス
                ai_tree->AddNode("Far", "PoisonBreath", 0, BehaviorTree::SelectRule::Non, nullptr, new PoisonAttackAction(this));
            }
        }
#endif
        // 怯み
        ai_tree->AddNode("Root", "Fear", 1, BehaviorTree::SelectRule::Non, new FearJudgment(this), new FearAction(this));
        // 死亡
        ai_tree->AddNode("Root", "Death", 0, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));

    }

}

void BossSpider::SetUpHitCollision()
{
    // 暗い当たり判定配列にノードをセット
    auto PutInCollisions = [this](const char* bone_name_, float node_radius_,std::vector<std::unique_ptr<NodeCollision>>&vec)
    {
        NodeCollision* node_collision;
        node_collision =new NodeCollision;
        node_collision->bone_name = bone_name_;
        node_collision->node_radius = node_radius_;

        vec.emplace_back(node_collision);
    };

    // todo 喰らいの当たり判定の量が少ない。岡君にリグ足してもらう
    /*------------------ 喰らい当たり判定のセット --------------*/
    PutInCollisions("J_head_end",    0.8f, hit_collisions);
    PutInCollisions("J_leg_A_03_L",  0.8f, hit_collisions);
    PutInCollisions("J_leg_A_end_L", 0.5f, hit_collisions);
    PutInCollisions("J_leg_B_03_L",  0.8f, hit_collisions);
    PutInCollisions("J_leg_B_end_L", 0.5f, hit_collisions);
    PutInCollisions("J_leg_C_03_L",  0.8f, hit_collisions);
    PutInCollisions("J_leg_C_end_L", 0.5f, hit_collisions);
    PutInCollisions("J_leg_D_03_L",  0.8f, hit_collisions);
    PutInCollisions("J_leg_D_end_L", 0.5f, hit_collisions);
    PutInCollisions("J_leg_A_03_R",  0.8f, hit_collisions);
    PutInCollisions("J_leg_A_end_R", 0.5f, hit_collisions);
    PutInCollisions("J_leg_B_03_R",  0.8f, hit_collisions);
    PutInCollisions("J_leg_B_end_R", 0.5f, hit_collisions);
    PutInCollisions("J_leg_C_03_R",  0.8f, hit_collisions);
    PutInCollisions("J_leg_C_end_R", 0.5f, hit_collisions);
    PutInCollisions("J_leg_D_03_R",  0.8f, hit_collisions);
    PutInCollisions("J_leg_D_end_R", 0.5f, hit_collisions);
    PutInCollisions("J_lowbody",     1.0f, hit_collisions);
    PutInCollisions("J_lowbody_end", 1.0f, hit_collisions);

    /*--------------- 両腕攻撃の当たり判定配列のセット ------------*/
    PutInCollisions("J_leg_A_01_L",  0.8f, arm_attack_collisions);
    PutInCollisions("J_leg_A_02_L",  0.8f, arm_attack_collisions);
    PutInCollisions("J_leg_A_03_L",  0.8f, arm_attack_collisions);
    PutInCollisions("J_leg_A_end_L", 0.5f, arm_attack_collisions);
    PutInCollisions("J_leg_A_01_R",  0.8f, arm_attack_collisions);
    PutInCollisions("J_leg_A_02_R",  0.8f, arm_attack_collisions);
    PutInCollisions("J_leg_A_03_R",  0.8f, arm_attack_collisions);
    PutInCollisions("J_leg_A_end_R", 0.5f, arm_attack_collisions);

}
