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
        position = Model->joint_position("spider_boss_spider_boss", "J_leg_A_03_L", &keyframe, world);
        debug_renderer->DrawSphere(position, attack_collision_range, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f));
    }
}

void BossSpider::BehaviorTreeInitialize()
{
    behaviorData = new BehaviorData();
    ai_tree = new BehaviorTree(this);

    ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
    {
        // ”ñí“¬
        ai_tree->AddNode("Root", "NonBattle", 2, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
        {
            // œpœj
            ai_tree->AddNode("NonBattle", "Wander", 0, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
            // ‘Ò‹@
            ai_tree->AddNode("NonBattle", "Idle", 1, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
        }
        // í“¬
        ai_tree->AddNode("Root", "Battle", 1, BehaviorTree::SelectRule::Priority,new BattleJudgment(this) , nullptr);
        {
            // ’ÇÕ
            ai_tree->AddNode("Battle", "Pursue", 2, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
            // UŒ‚
            ai_tree->AddNode("Battle", "Attack", 1, BehaviorTree::SelectRule::Random, new AttackJudgment(this), nullptr);
            {
                ai_tree->AddNode("Attack", "NearAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new NearAttackAction(this));
            }
        }
        // Ž€–S
        ai_tree->AddNode("Root", "Death", 0, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));

    }

}
