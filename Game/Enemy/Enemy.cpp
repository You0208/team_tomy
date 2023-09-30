#include "Enemy.h"

#include <directxmath.h>

#include "imgui.h"
#include "Game/AI/ActionDerived.h"
#include "Game/Manager/CharacterManager.h"
#include "Game/Scene/SceneGame.h"
#include "Lemur/Graphics/Camera.h"
#include "Lemur/Input/GamePad.h"
#include "Lemur/Input/Input.h"


void EnemyGraphicsComponent::Initialize(GameObject* gameobj)
{
    Enemy* demoPlayer = dynamic_cast<Enemy*> (gameobj);
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    demoPlayer->SetModel(ResourceManager::Instance().LoadModelResource(graphics.GetDevice(), ".\\resources\\Enemy\\arakBarrak_v025.fbx"));
}

void EnemyGraphicsComponent::Update(GameObject* gameobj)
{
    Enemy* demoPlayer = dynamic_cast<Enemy*> (gameobj);

    ImGui::Begin("Player");
    if (ImGui::TreeNode("Transform"))
    {
        DirectX::XMFLOAT3 pos = demoPlayer->GetPosition();
        ImGui::DragFloat3("position", &pos.x);
        demoPlayer->SetPosition(pos);

        ImGui::TreePop();
    }
    ImGui::End();
}

void EnemyGraphicsComponent::Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader)
{
    Enemy* enemy = dynamic_cast<Enemy*> (gameobj);
    
    enemy->Render(elapsedTime, replaced_pixel_shader);
}

void Enemy::BehaviorTreeInitialize()
{
    ai_tree = new BehaviorTree(this);

    ai_tree->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
    {
        // 非戦闘
        ai_tree->AddNode("Root", "NonBattle", 1, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
        {
            // 徘徊
            ai_tree->AddNode("NonBattle", "Wander", 1, BehaviorTree::SelectRule::Non, nullptr, new WanderAction(this));
            //todo 待機

        }
        // 戦闘
        //ai_tree->AddNode("Root","Battle",0,BehaviorTree::SelectRule::Priority,,nullptr)
        // todo 三日ったら追跡する谷津作
    }


}

void Enemy::Turn(float vx, float vz, float speed)
{
    //// todo 時間取ってこれるようにSceneManagerに入れる。
    //speed *= GameScene::timer;

    //// ベクトルの大きさを取得
    //float Length = sqrtf(vx * vx + vz * vz);

    //// ベクトルの大きさが0なら(ゼロベクトルなら)
    //if (Length <= 0.01)
    //{
    //    return;
    //}

    //// 進行ベクトルの正規化
    //vx = vx / Length;
    //vz = vz / Length;

    //// 自身の回転値から前方向を求める。
    //float frontX = sinf(angle.y);
    //float frontZ = cosf(angle.y);

    //// 回転角を求めるために、2つの単位ベクトルの内積を計算する
    //float dot = (vx * frontX) + (vz * frontZ);

    //// dot は -1.0f ～ 1.0f になる。なので rot は 0.0f ～ 2.0f になる。
    //float rot = 1.0f - dot;

    //// 内積が小さくなったら
    //if (rot < speed) speed = rot; // その分向きを変える角度も小さくする

    //// 左右判定を行うために2つの単位ベクトルの外積を計算する
    //float cross = (vx * frontZ) - (vz * frontX);

    //// 2Dの外積値が正の場合か負の場合によって左右反転が行える
    //// 左右判定を行うことによって左右回転を選択する
    //if (cross < 0.0f)
    //{
    //    angle.y -= speed;
    //}
    //else
    //{
    //    angle.y += speed;
    //}


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
    //float vx = target_position.x - position.x;
    //float vz = target_position.z - position.z;
    //float dist = sqrtf((vx * vx) + (vz * vz));

    //// 正規化
    //vx /= dist;
    //vz /= dist;

    //Move(vx, vz, base_move_speed * move_speed_rate);
    //Turn(vx, vz, turnSpeed * turn_speed_rate);

}

void Enemy::SetRandomTargetPosition()
{
    //// territory_rangeの大きさの円でランダムな値をとる。
    //float theta = Argent::Random::Generate(-DirectX::XM_PI, DirectX::XM_PI);
    //float range = Argent::Random::Generate(0.0f, territory_range);

    //target_position.x = territory_origin.x + sinf(theta) * range;
    //target_position.y = territory_origin.y;
    //target_position.z = territory_origin.z + cosf(theta) * range;

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

    //enemy->

}
