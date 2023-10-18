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
        position = Model->joint_position("spider_boss_spider_boss", "J_root", &keyframe, world);
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
        ai_tree->AddNode("Root", "NonBattle", 3, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
        {
            // œpœj
            ai_tree->AddNode("NonBattle", "Wander", 0, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
            // ‘Ò‹@
            ai_tree->AddNode("NonBattle", "Idle", 1, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
        }
        // í“¬
        ai_tree->AddNode("Root", "Battle", 2, BehaviorTree::SelectRule::Priority,new BattleJudgment(this) , nullptr);
        {
            // Ž²‡‚í‚¹
            ai_tree->AddNode("Battle", "Turn", 0, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
            // ‹ßÚí“¬
            ai_tree->AddNode("Battle", "Near", 1, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
            {
                // ”ò‚Ñ‚Â‚«¨“ñ’iUŒ‚‚ÌƒRƒ“ƒ{
                ai_tree->AddNode("Near", "Combo_A", 0, BehaviorTree::SelectRule::Sequence, nullptr, nullptr);
                {
                    ai_tree->AddNode("Combo_A", "JumpAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new JumpAttackAction(this));
                }
                // ‘Ò‹@orŽ²‡‚í‚¹¨“ñ’iUŒ‚or—¼˜rUŒ‚or”ò‚Ñ‚Â‚«UŒ‚‚ÌƒRƒ“ƒ{
                ai_tree->AddNode("Near", "Combo_B", 0, BehaviorTree::SelectRule::Sequence, nullptr, nullptr);
                {
                    // ‘Ò‹@‚©Ž²‡‚í‚¹
                    ai_tree->AddNode("Combo_B", "Idle_or_Turn", 0, BehaviorTree::SelectRule::Random, nullptr, nullptr);
                    {
                        ai_tree->AddNode("Idle_or_Turn", "Idle", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
                        ai_tree->AddNode("Idle_or_Turn", "Turn", 0, BehaviorTree::SelectRule::Non, nullptr, new TuraAction(this));
                    }
                    // “ñ’iUŒ‚‚©—¼˜rUŒ‚‚©”ò‚Ñ‚Â‚«UŒ‚
                    ai_tree->AddNode("Combo_B", "DA_or_TA_or_JA", 0, BehaviorTree::SelectRule::Random, nullptr, nullptr);
                    {
                        ai_tree->AddNode("DA_or_TA_or_JA", "DoubleAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new DoubleAttackAction(this));
                        ai_tree->AddNode("DA_or_TA_or_JA", "TwinArmsAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new TwinArmsAttackAction(this));
                        ai_tree->AddNode("DA_or_TA_or_JA", "JumpAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new JumpAttackAction(this));
                    }
                }
                // ƒoƒbƒNƒXƒeƒbƒv
                ai_tree->AddNode("Near", "BackStep", 0, BehaviorTree::SelectRule::Non, nullptr, new BackStepAction(this));
                // UŒ‚
                ai_tree->AddNode("Near", "Attack", 1, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
                {
                    ai_tree->AddNode("Attack", "NearAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new NearAttackAction(this));
                }
            }
            // ’†‹——£í“¬
            ai_tree->AddNode("Battle", "Middle", 2, BehaviorTree::SelectRule::Random, new MiddleJudgment(this), nullptr);
            {
                // ’ÇÕ
                ai_tree->AddNode("Middle", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
                // “ÅƒuƒŒƒX
                ai_tree->AddNode("Middle", "PoisonBreath", 0, BehaviorTree::SelectRule::Non, nullptr, new PoisonAttackAction(this));

            }
            // ‰“‹——£í“¬
            ai_tree->AddNode("Battle", "Far", 3, BehaviorTree::SelectRule::Random, nullptr, nullptr);
            {
                // “ËiUŒ‚
                ai_tree->AddNode("Far", "Rush", 0, BehaviorTree::SelectRule::Non, nullptr, new RushAttackAction(this));
                    // “ÅƒuƒŒƒX
                ai_tree->AddNode("Far", "PoisonBreath", 0, BehaviorTree::SelectRule::Non, nullptr, new PoisonAttackAction(this));
            }
        }
        // ‹¯‚Ý
        ai_tree->AddNode("Root", "Fear", 1, BehaviorTree::SelectRule::Non, new FearJudgment(this), new FearAction(this));
        // Ž€–S
        ai_tree->AddNode("Root", "Death", 0, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));

    }

}
