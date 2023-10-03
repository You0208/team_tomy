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

void EnemyGraphicsComponent::Initialize(GameObject* gameobj)
{
    Enemy* demoPlayer = dynamic_cast<Enemy*> (gameobj);
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    demoPlayer->SetModel(ResourceManager::Instance().LoadModelResource(graphics.GetDevice(), ".\\resources\\Enemy\\arakBarrak_v025.fbx"));
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

void Enemy::BehaviorTreeInitialize()
{
    behaviorData = new BehaviorData();
    ai_tree = new BehaviorTree(this);

    ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
    {
        // 非戦闘
        ai_tree->AddNode("Root", "NonBattle", 1, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
        {
            // 徘徊
            ai_tree->AddNode("NonBattle", "Wander", 0, BehaviorTree::SelectRule::Non, new WanderJudgment(this), new WanderAction(this));
            // 待機
            ai_tree->AddNode("NonBattle", "Idle", 1, BehaviorTree::SelectRule::Non, nullptr, new IdleAction(this));
        }
        // 戦闘
        ai_tree->AddNode("Root", "Battle", 0, BehaviorTree::SelectRule::Priority,new BattleJudgment(this) , nullptr);
        {
            // 追跡
            ai_tree->AddNode("Battle", "Pursue", 0, BehaviorTree::SelectRule::Non, nullptr, new PursueAction(this));
            // 死亡
            ai_tree->AddNode("Battle", "Death", 1, BehaviorTree::SelectRule::Non, new DeathJudgment(this), new DeathAction(this));
        }
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


bool Enemy::ReachTargetJudge(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target_pos, float judge_range)
{
    float vx = target_position.x - position.x;
    float vz = target_position.z - position.z;

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
    Move(vx, vz, GetWalkSpeed());
    Turn(vx, vz, 3.0f);

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

        ImGui::DragFloat("attack_range", &attack_range);

        DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();
        float enemy_to_player = Length(position, player_pos);
        ImGui::DragFloat("enemy_to_player", &enemy_to_player);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("parameter"))
    {
        ImGui::DragInt("MaxHealth", &maxHealth);
        ImGui::DragInt("health", &health);
        ImGui::TreePop();
    }

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
