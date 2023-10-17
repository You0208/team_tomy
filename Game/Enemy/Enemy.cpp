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
    switch (enemy->enemy_type)
    {
    case Enemy::SmallSpider:
        enemy->SetModel(ResourceManager::Instance().LoadModelResource(graphics.GetDevice(), ".\\resources\\Enemy\\spider_v003.fbx"));
        enemy->meshName = "polySurface";

        break;
    case Enemy::BossSpider:
        enemy->SetModel(ResourceManager::Instance().LoadModelResource(graphics.GetDevice(), ".\\resources\\Enemy\\spiderboss_v003.fbx"));

        enemy->meshName = "spider_boss_spider_boss";
        break;
    }
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

//void Enemy::DrawDebugPrimitive()
//{
//    //DebugRenderer* debug_renderer = Lemur::Graphics::Graphics::Instance().GetDebugRenderer();
//    //DirectX::XMFLOAT3 position = Model->joint_position("polySurface1","J_root" , &keyframe, world);
//    //debug_renderer->DrawSphere(position, radius,  DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f));
//
//    //if(attack_collision_flag)
//    //{
//    //    position = Model->joint_position("polySurface1", "J_leg_A_03_L", &keyframe, world);
//    //    debug_renderer->DrawSphere(position, attack_collision_range, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f));
//    //}
//}

//void Enemy::BehaviorTreeInitialize()
//{
//    behaviorData = new BehaviorData();
//    ai_tree = new BehaviorTree(this);
//
//    ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
//    {
//        // 非戦闘
//        ai_tree->AddNode("Root", "NonBattle", 2, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
//        {
//            // 徘徊
//            ai_tree->AddNode("NonBattle", "Wander", 0, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
//            // 待機
//            ai_tree->AddNode("NonBattle", "Idle", 1, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
//        }
//        // 戦闘
//        ai_tree->AddNode("Root", "Battle", 1, BehaviorTree::SelectRule::Priority,new BattleJudgment(this) , nullptr);
//        {
//            // 追跡
//            ai_tree->AddNode("Battle", "Pursue", 2, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
//            // 攻撃
//            ai_tree->AddNode("Battle", "Attack", 1, BehaviorTree::SelectRule::Random, new AttackJudgment(this), nullptr);
//            {
//                ai_tree->AddNode("Attack", "NearAttack", 0, BehaviorTree::SelectRule::Non, nullptr, new NearAttackAction(this));
//            }
//        }
//        // 死亡
//        ai_tree->AddNode("Root", "Death", 0, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));
//
//    }
//
//
//}

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
bool Enemy::ReachTargetJudge(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target_pos, float judge_range)
{
    float vx = target_pos.x - pos.x;
    float vz = target_pos.z - pos.z;

    // ターゲット位置までの距離(二乗)
    float dist_sq = ((vx * vx) + (vz * vz));
    judge_range *= 2.0f;

    if (dist_sq < judge_range)
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
    Move(vx, vz, walk_speed);
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
        // todo このif分の前に内積判定してカウンター成功か判定する
        if (player->CounterJudge(this)){}
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

void Enemy::DebugImgui()
{
    ImGui::Begin("Enemy");
    if (ImGui::TreeNode("Transform"))
    {
        ImGui::DragFloat3("position", &position.x);
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
        ImGui::DragFloat("attack_power", &attack_power);
        ImGui::DragFloat("defense_power", &defense_power);
        ImGui::TreePop();
    }
    ImGui::Checkbox("is_hit", &is_hit);

    ImGui::End();

}

void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
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

}

void EnemyPhysicsComponent::Update(GameObject* gameobj, float elapsedTime)
{
    Enemy* enemy = dynamic_cast<Enemy*> (gameobj);

    enemy->BehaviorTreeUpdate();

    enemy->UpdateVelocity(elapsedTime);
}
