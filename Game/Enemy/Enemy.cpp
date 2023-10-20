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
#if 0 デバッグ用
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
            ai_tree->AddNode("NonBattle", "Idle", 1, BehaviorTree::SelectRule::Non, nullptr, new TuraAction(this));
        }
        // 戦闘
        ai_tree->AddNode("Root", "Battle", 2, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), nullptr);
        {
            // 軸合わせ
            ai_tree->AddNode("Battle", "Turn", 0, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
            // 近接戦闘
            ai_tree->AddNode("Battle", "Near", 1, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
            {
                // 飛びつき攻撃
                ai_tree->AddNode("Near", "JumpAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new JumpAttackAction(this));
                //待機
                ai_tree->AddNode("Near", "Idle", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
            }
            // 中距離戦闘
            ai_tree->AddNode("Battle", "Middle", 2, BehaviorTree::SelectRule::Random, new MiddleJudgment(this), nullptr);
            {
                // 追跡
                ai_tree->AddNode("Middle", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
            }
            // 遠距離戦闘
            ai_tree->AddNode("Battle", "Far", 3, BehaviorTree::SelectRule::Random, nullptr, nullptr);
            {
                // 追跡
                ai_tree->AddNode("Far", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
            }
        }
#endif
        // 死亡
        ai_tree->AddNode("Root", "Death", 0, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));

    }

}

void Enemy::BehaviorTreeInitialize_Level2()
{
    behaviorData = new BehaviorData();
    ai_tree = new BehaviorTree(this);

    ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
    {
#if 0 デバッグ用
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
        ai_tree->AddNode("Root", "Battle", 2, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), nullptr);
        {
            // 軸合わせ
            ai_tree->AddNode("Battle", "Turn", 0, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
            // 近接戦闘
            ai_tree->AddNode("Battle", "Near", 1, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
            {
                // 待機or軸合わせ→両腕攻撃のコンボ
                ai_tree->AddNode("Near", "Combo_B", 0, BehaviorTree::SelectRule::Sequence, nullptr, nullptr);
                {
                    // 待機か軸合わせ
                    ai_tree->AddNode("Combo_B", "Idle_or_Turn", 0, BehaviorTree::SelectRule::Random, nullptr, nullptr);
                    {
                        ai_tree->AddNode("Idle_or_Turn", "Idle", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
                        ai_tree->AddNode("Idle_or_Turn", "Turn", 0, BehaviorTree::SelectRule::Non, nullptr, new TuraAction(this));
                    }
                    //両腕攻撃
                    ai_tree->AddNode("Combo_B", "TwinArmsAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new TwinArmsAttackAction(this));
                }
                //両腕攻撃
                ai_tree->AddNode("Near", "TwinArmsAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new TwinArmsAttackAction(this));
                // バックステップ
                ai_tree->AddNode("Near", "BackStep", 0, BehaviorTree::SelectRule::Non, nullptr, new BackStepAction(this));
            }
            // 中距離戦闘
            ai_tree->AddNode("Battle", "Middle", 2, BehaviorTree::SelectRule::Random, new MiddleJudgment(this), nullptr);
            {
                // 追跡
                ai_tree->AddNode("Middle", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
            }
            // 遠距離戦闘
            ai_tree->AddNode("Battle", "Far", 3, BehaviorTree::SelectRule::Random, nullptr, nullptr);
            {
                // 追跡
                ai_tree->AddNode("Far", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
            }
        }
#endif
        // 死亡
        ai_tree->AddNode("Root", "Death", 0, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));

    }

}

void Enemy::BehaviorTreeInitialize_Level3()
{
    behaviorData = new BehaviorData();
    ai_tree = new BehaviorTree(this);

    ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
    {
#if 0 デバッグ用
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
        ai_tree->AddNode("Root", "Battle", 2, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), nullptr);
        {
            // 軸合わせ
            ai_tree->AddNode("Battle", "Turn", 0, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
            // 近接戦闘
            ai_tree->AddNode("Battle", "Near", 1, BehaviorTree::SelectRule::Random, new NearJudgment(this), nullptr);
            {
                // 飛びつき攻撃
                ai_tree->AddNode("Near", "JumpAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new JumpAttackAction(this));
            
                // 待機or軸合わせ→飛びつき攻撃のコンボ
                ai_tree->AddNode("Near", "Combo_B", 0, BehaviorTree::SelectRule::Sequence, nullptr, nullptr);
                {
                    // 待機か軸合わせ
                    ai_tree->AddNode("Combo_B", "Idle_or_Turn", 0, BehaviorTree::SelectRule::Random, nullptr, nullptr);
                    {
                        ai_tree->AddNode("Idle_or_Turn", "Idle", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
                        ai_tree->AddNode("Idle_or_Turn", "Turn", 0, BehaviorTree::SelectRule::Non, nullptr, new TuraAction(this));
                    }
                    //飛びつき攻撃
                    ai_tree->AddNode("Combo_B", "JumpAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new JumpAttackAction(this));
                }
                // バックステップ
                ai_tree->AddNode("Near", "BackStep", 0, BehaviorTree::SelectRule::Non, nullptr, new BackStepAction(this));
            }
            // 中距離戦闘
            ai_tree->AddNode("Battle", "Middle", 2, BehaviorTree::SelectRule::Random, new MiddleJudgment(this), nullptr);
            {
                // 毒ブレス
                ai_tree->AddNode("Middle", "PoisonBreath", 0, BehaviorTree::SelectRule::Non, nullptr, new PoisonAttackAction(this));

            }
            // 遠距離戦闘
            ai_tree->AddNode("Battle", "Far", 3, BehaviorTree::SelectRule::Random, nullptr, nullptr);
            {
                // 追跡
                ai_tree->AddNode("Far", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
                // 毒ブレス
                ai_tree->AddNode("Far", "PoisonBreath", 0, BehaviorTree::SelectRule::Non, nullptr, new PoisonAttackAction(this));
            }
        }
#endif
        // 死亡
        ai_tree->AddNode("Root", "Death", 0, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));

    }

}

void Enemy::BehaviorTreeInitialize_Level4()
{
    behaviorData = new BehaviorData();
    ai_tree = new BehaviorTree(this);

    ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
    {
#if 0 デバッグ用
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
        ai_tree->AddNode("Root", "Battle", 2, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), nullptr);
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
        // 死亡
        ai_tree->AddNode("Root", "Death", 0, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));

    }

}

void Enemy::BehaviorTreeInitialize_Level5()
{
    behaviorData = new BehaviorData();
    ai_tree = new BehaviorTree(this);

    ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
    {
#if 1 デバッグ用
        //// 待機(デバッグ用)
        //ai_tree->AddNode("Root", "Idle(debug)", 0, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
                        // バックステップ
        ai_tree->AddNode("Root", "BackStep", 0, BehaviorTree::SelectRule::Non, nullptr, new BackStepAction(this));

        //// 両腕攻撃(デバッグ用)
        //ai_tree->AddNode("Root", "TwinArmsAttack(debug)", 3, BehaviorTree::SelectRule::Non, nullptr, new TwinArmsAttackAction(this));
        //// 軸合わせ(デバッグ用)
        //ai_tree->AddNode("Root", "Turn(debug)", 2, BehaviorTree::SelectRule::Non, new TurnJudgment(this), new TuraAction(this));
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
        ai_tree->AddNode("Root", "Battle", 2, BehaviorTree::SelectRule::Priority, new BattleJudgment(this), nullptr);
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

void Enemy::BehaviorTreeUpdate()
{
    // 現在実行されているノードが無ければ
    if (activeNode == nullptr)
    {
        // 次に実行するノードを推論する。
        activeNode = ai_tree->ActiveNodeInference(behaviorData);
    }
    // 現在実行するノードがあれば
    if (activeNode != nullptr)
    {
        // ビヘイビアツリーからノードを実行。
        activeNode = ai_tree->Run(activeNode, behaviorData, high_resolution_timer::Instance().time_interval());
    }

}

// todo これモジュール化してNeroに入れる。
bool Enemy::DistanceJudge(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target_pos, float judge_range)
{
    //float vx = target_pos.x - pos.x;
    //float vz = target_pos.z - pos.z;

    //// ターゲット位置までの距離(二乗)
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
    // プレイヤーの位置
    DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();

    // x軸距離
    float vec_x = player_pos.x - position.x;
    // z軸距離
    float vec_z = player_pos.z - position.z;
    // 合計距離(二乗)
    float length_sq = (vec_x * vec_x) + (vec_z * vec_z);

    // 距離が範囲以内なら
    if (length_sq < found_distance * found_distance)
    {
        //次は正面判定

        // 前方向Xベクトル
        float front_x = sinf(rotation.y);
        // 前方向Zベクトル
        float front_z = cosf(rotation.y);

        // 二乗を外す
        float length = sqrtf(length_sq);
        // 正規化
        vec_x /= length;
        vec_z /= length;

        // 内積
        float dot = (front_x * vec_x) + (front_z * vec_z);
        // 正面に居たら
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

    // 正規化
    vx /= dist;
    vz /= dist;

    // todo ここら辺も今はテキトー
    Move(vx, vz, walk_speed *speed_power* move_speed_rate);
    Turn(vx, vz, 5.0f);

}

void Enemy::CollisionNodeVsPlayer(const char* mesh_name, const char* bone_name, float node_radius)
{
    Player* player = CharacterManager::Instance().GetPlayer();

    // 無敵中なら判定しない
    if (player->invincible)
        return;

    // ノード位置取得
    DirectX::XMFLOAT3 nodePosition = Model->joint_position(mesh_name, bone_name, &keyframe, world);

    if (Collision::IntersectSphereVsCylinder(
        nodePosition, node_radius,
        player->GetPosition(), player->GetRadius(), player->GetHeight())
        )
    {
        if (player->CounterJudge(nodePosition)){}
        else if (player->ApplyDamage(attack_power))
        {
            // 豆腐スキル持ってたら強制的にHP0にする
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
    // territory_rangeの大きさの円でランダムな値をとる。
    float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
    float range = Mathf::RandomRange(0.0f, territory_range);

    target_position.x = territory_origin.x + sinf(theta) * range;
    target_position.y = territory_origin.y;
    target_position.z = territory_origin.z + cosf(theta) * range;

}

void Enemy::DamageRender(const float damage)
{
    // ビューポート
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    ID3D11DeviceContext* dc = Lemur::Graphics::Graphics::Instance().GetDeviceContext();
    dc->RSGetViewports(&numViewports, &viewport);

    // 変換行列 (計算しやすいように行列に変換)
    DirectX::XMMATRIX V = Camera::Instance().GetViewMatrix();
    DirectX::XMMATRIX P = Camera::Instance().GetProjectionMatrix();
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    // ベクトル変換
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
    // 暗い当たり判定配列にノードをセット
    auto PutInCollisions = [this](const char* bone_name_, float node_radius_, std::vector<std::unique_ptr<NodeCollision>>& vec)
    {
        NodeCollision* node_collision;
        node_collision = new NodeCollision;
        node_collision->bone_name = bone_name_;
        node_collision->node_radius = node_radius_;

        vec.emplace_back(node_collision);
    };

    // todo 喰らいの当たり判定の量が少ない。岡君にリグ足してもらう
    // todo なんかノードおかしい
    /*------------------ 喰らい当たり判定のセット --------------*/
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

    /*--------------- 両腕攻撃の当たり判定配列のセット ------------*/
    PutInCollisions("J_leg_A_01_L", 0.2f, arm_attack_collisions);
    PutInCollisions("J_leg_A_02_L", 0.2f, arm_attack_collisions);
    PutInCollisions("J_leg_A_03_L", 0.2f, arm_attack_collisions);
    //PutInCollisions("J_leg_A_end_L", 0.5f, arm_attack_collisions);
    PutInCollisions("J_leg_A_01_R", 0.2f, arm_attack_collisions);
    PutInCollisions("J_leg_A_02_R", 0.2f, arm_attack_collisions);
    PutInCollisions("J_leg_A_03_R", 0.2f, arm_attack_collisions);
    //PutInCollisions("J_leg_A_end_R", 0.5f, arm_attack_collisions);

}


// 入力処理
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

    // カメラ方向とステイックの入力値によって進行方向を計算する。

    /*--------------- カメラ方向の計算 ---------------*/

    Camera& camera = Camera::Instance();

    DirectX::XMMATRIX V = camera.GetViewMatrix();
    DirectX::XMMATRIX W = DirectX::XMMatrixInverse(nullptr, V);
    DirectX::XMFLOAT4X4 w;
    DirectX::XMStoreFloat4x4(&w, W);

    const DirectX::XMFLOAT3& cameraRight = { w._11,w._12,w._13 };
    const DirectX::XMFLOAT3& cameraFront = { w._31,w._32,w._33 };

    // 移動ベクトルはXZ平面に水平なベクトルになるようにする。

    // カメラ右方向ベクトルをXZ単位ベクトルに変換。
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    const float cameraRightLength = sqrtf(
        (cameraRight.x * cameraRight.x) + cameraRight.z * cameraRight.z
    );
    if (cameraRightLength > 0.0f)
    {
        // 単位ベクトル化(正規化)
        cameraRightX = cameraRightX / cameraRightLength;
        cameraRightZ = cameraRightZ / cameraRightLength;
    }

    //カメラ前方向のベクトルをXZ単位ベクトルに変換。
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(
        (cameraFront.x * cameraFront.x) + cameraFront.z * cameraFront.z
    );
    if (cameraFrontLength > 0.0f)
    {
        //単位ベクトル化(正規化)
        cameraFrontX = cameraFrontX / cameraFrontLength;
        cameraFrontZ = cameraFrontZ / cameraFrontLength;
    }

    // スティックの水平入力値をカメラ右方向に反映し、
    //スティックの垂直入力値をカメラ前方向に反映し、
    //進行ベクトルを計算する。
    DirectX::XMFLOAT3 vec{};
    vec.x = cameraFrontX * sinf(input_y) + cameraRightX * sinf(input_x);
    vec.z = cameraFrontZ * sinf(input_y) + cameraRightZ * sinf(input_x);
    //y軸方向には移動しない
    vec.y = 0.0f;


    return vec;

}

void EnemyPhysicsComponent::Initialize(GameObject* gameobj)
{
    Enemy* enemy = dynamic_cast<Enemy*> (gameobj);

    // todo ここでテリトリーを設定(とりあえず今は原点)
    
    enemy->BehaviorTreeInitialize();
    enemy->SetUpHitCollision();
}

void EnemyPhysicsComponent::Update(GameObject* gameobj, float elapsedTime)
{
    Enemy* enemy = dynamic_cast<Enemy*> (gameobj);

    enemy->BehaviorTreeUpdate();

    enemy->UpdateVelocity(elapsedTime);
}
