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
        // ��퓬
        ai_tree->AddNode("Root", "NonBattle", 3, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
        {
            // �p�j
            ai_tree->AddNode("NonBattle", "Wander", 0, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
            // �ҋ@
            ai_tree->AddNode("NonBattle", "Idle", 1, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
        }
        // �퓬
        ai_tree->AddNode("Root", "Battle", 2, BehaviorTree::SelectRule::Priority,new BattleJudgment(this) , nullptr);
        {
            // �ߐڐ퓬
            ai_tree->AddNode("Battle", "Near", 0, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
            {
                //// ��т�����i�U���̃R���{
                //ai_tree->AddNode("Near", "Combo_A", 0, BehaviorTree::SelectRule::Sequence, nullptr, nullptr);
                //{
                //    ai_tree->AddNode("Combo_A", "JumpAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new JumpAttackAction(this));
                //}
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
                //// �o�b�N�X�e�b�v
                //ai_tree->AddNode("Near", "BackStep", 0, BehaviorTree::SelectRule::Non, nullptr, new BackStepAction(this));
                //// �U��
                //ai_tree->AddNode("Near", "Attack", 1, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
                //{
                //    ai_tree->AddNode("Attack", "NearAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new NearAttackAction(this));
                //}
            }
            // �������퓬
            ai_tree->AddNode("Battle", "Middle", 1, BehaviorTree::SelectRule::Random, new MiddleJudgment(this), nullptr);
            {
                // �ǐ�
                ai_tree->AddNode("Middle", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
            }
        }
        // ����
        ai_tree->AddNode("Root", "Fear", 1, BehaviorTree::SelectRule::Non, new FearJudgment(this), new FearAction(this));
        // ���S
        ai_tree->AddNode("Root", "Death", 0, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));

    }

}
