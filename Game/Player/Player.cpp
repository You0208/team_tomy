#include "Player.h"

#include <directxmath.h>

#include "imgui.h"
#include "Game/Manager/CharacterManager.h"
#include "Game/Manager/EnemyManager.h"
#include "Lemur/Graphics/Camera.h"
#include "Lemur/Graphics/framework.h"
#include "Game/StateMachine/StateDerived.h"
#include "Game/StateMachine/StateMachine.h"
#include "Lemur/Collision/Collision.h"

void PlayerGraphicsComponent::Initialize(GameObject* gameobj)
{
    Player* player = dynamic_cast<Player*> (gameobj);
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    player->SetModel(ResourceManager::Instance().LoadModelResource(graphics.GetDevice(), ".\\resources\\Player\\player_v009.fbx"));
}

void PlayerGraphicsComponent::Update(GameObject* gameobj)
{
}

void PlayerGraphicsComponent::Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader)
{
    Player* player = dynamic_cast<Player*> (gameobj);

    player->Render(elapsedTime, replaced_pixel_shader);

    player->DebugImgui();
}

void Player::DebugImgui()
{
    ImGui::Begin("Player");
    ImGui::DragInt("down_count", &down_count);
    if(ImGui::TreeNode("Transform"))
    {
        ImGui::DragFloat3("position", &position.x);
        ImGui::DragFloat("scale_factor", &scaleFactor);
        ImGui::DragFloat("height", &height);
        ImGui::DragFloat("radius", &radius);

        ImGui::TreePop();
    }
    if(ImGui::TreeNode("State"))
    {
        ImGui::Text(state_machine->GetCurrentState()->state_name.c_str());
        state_machine->GetCurrentState()->DrawImGui();
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("parameter"))
    {
        ImGui::DragFloat("walk_speed", &walk_speed);
        ImGui::DragFloat("turn_speed", &turn_speed);

        ImGui::DragInt("MaxHealth", &max_health);
        ImGui::DragInt("health", &health);
        if (health <= 0)
        {
            health = 0;
            death = true;
        }

        ImGui::DragFloat("attack_power", &attack_power);
        ImGui::DragFloat("defense_power", &defense_power);
        ImGui::DragFloat("speed_power", &speed_power);

        ImGui::DragFloat("bet_AP", &bet_AP);
        ImGui::DragFloat("bet_DP", &bet_DP);
        ImGui::DragFloat("bet_SP", &bet_SP);
        ImGui::DragFloat("bet_MHP", &bet_MHP);
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("Skills"))
    {
        for(auto& skill:skills)
        {
            if(ImGui::TreeNode(skill->GetName().c_str()))
            {
                skill->DrawImGui();
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("Attack & Hit"))
    {
        ImGui::DragFloat("near_attack_range", &attack_collision_range);
        ImGui::Checkbox("attack_collision_flag", &attack_collision_flag);
        ImGui::Checkbox("is_hit_stop", &is_hit_stop);
        ImGui::Checkbox("invincible", &invincible);
        ImGui::DragInt("invincible_frame", &invincible_frame);

        ImGui::TreePop();
    }
    if(ImGui::TreeNode("Animation"))
    {
        ImGui::InputInt("animation_index", &animation_index);
        ImGui::InputFloat("animation_tick", &animation_tick);
        ImGui::DragInt("frame_index", &frame_index);
        ImGui::Checkbox("end_animation", &end_animation);
        ImGui::Checkbox("animStop", &animStop);
        ImGui::TreePop();
    }
    ImGui::End();
}

void Player::DrawDebugPrimitive()
{
    DebugRenderer* debug_renderer = Lemur::Graphics::Graphics::Instance().GetDebugRenderer();
    DirectX::XMFLOAT4 color{0.0f, 1.0f, 0.0f, 0.0f};

    // 無敵時は青で描画
    if (invincible)
        color = { 0.0f,0.0f,1.0f,0.0f };

    debug_renderer->DrawCylinder(position, radius, height, color);

    if(attack_collision_flag)
    {
        DirectX::XMFLOAT3 position = Model->joint_position("wepon", "J_wepon", &keyframe, world);
        debug_renderer->DrawSphere(position, attack_collision_range, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f));
    }
}

void Player::StateMachineInitialize()
{
    state_machine = new Nero::Component::AI::StateMachine;
    state_machine->SetUpState<Nero::Component::AI::IdleState>(this, true);
    state_machine->SetUpState<Nero::Component::AI::WalkState>(this);
    state_machine->SetUpState<Nero::Component::AI::AvoidState>(this);
    state_machine->SetUpState<Nero::Component::AI::AttackState>(this);
    state_machine->SetUpState<Nero::Component::AI::DeathState>(this);
    state_machine->SetUpState<Nero::Component::AI::FearState>(this);
    state_machine->SetUpState<Nero::Component::AI::SPAttackState>(this);
}

void Player::StateMachineUpdate()
{
    state_machine->Update();
}

bool Player::InputMove()
{
    GamePad& game_pad = Input::Instance().GetGamePad();
    float lx = game_pad.GetAxisLX();
    float ly = game_pad.GetAxisLY();

    DirectX::XMFLOAT3 vec = GetMoveVec(lx, ly);

    // 進行ベクトルがゼロベクトルならは入力されてない
    if (vec.x == 0.0f || vec.z == 0.0f)
        return false;

    Move(vec.x, vec.z, walk_speed * speed_power);
    Turn(vec.x, vec.z, turn_speed);

    return true;
}

bool Player::HaveSkill(const char* search_skill_name)
{
    for(auto& skill: skills)
    {
        if (skill->GetName() == search_skill_name)
            return true;
    }
    return false;
}

void Player::CalcSPAttackTime()
{
    if (SP_attack_cool_timer_ms >= SP_attack_cool_time_ms)
    {
        can_SP_attack = true;
    }
    else
    {
        SP_attack_cool_timer_ms += high_resolution_timer::Instance().time_interval();
    }
}

bool Player::CounterJudge(Enemy* enemy)
{
    if (!can_counter)return false;

    // 回転行列の作成
    DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    // 前方向ベクトル取得
    DirectX::XMVECTOR Forward = rotation_matrix.r[2];

    //プレイヤーの位置
    DirectX::XMVECTOR Player_pos = DirectX::XMLoadFloat3(&position);
    // 敵の位置
    DirectX::XMVECTOR Enemy_pos = DirectX::XMLoadFloat3(&enemy->GetPosition());
    // プレイヤーから敵までのベクトル
    DirectX::XMVECTOR Player_to_Enemy = DirectX::XMVectorSubtract(Enemy_pos, Player_pos);

    // 正規化
    Forward = DirectX::XMVector3Normalize(Forward);
    Player_to_Enemy = DirectX::XMVector3Normalize(Player_to_Enemy);

    // 内積で角度取得
    float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(Forward, Player_to_Enemy));

    if (dot > 0) 
    {
        is_counter = true;
        return true;
    }
    return false;
}

void Player::RetentionParamSet()
{
    retention_basicAP  = attack_power;
    retention_basicDP  = defense_power;
    retention_basicSP  = speed_power;
    retention_basicMHP = max_health;
}

void Player::RetentionParamGet()
{
    attack_power  = retention_basicAP;
    defense_power = retention_basicDP;
    speed_power   = retention_basicSP;
    max_health    = retention_basicMHP;
    if (health > max_health)
        health = max_health;
}

void Player::CollisionNodeVsEnemies(const char* mesh_name,const char* bone_name, float node_radius)
{

    // ノード位置取得
    DirectX::XMFLOAT3 nodePosition = Model->joint_position(mesh_name, bone_name, &keyframe, world);

    // 指定のノードと全ての敵を総当たりで衝突処理

    EnemyManager& enemy_manager = EnemyManager::Instance();
    int enemy_count = enemy_manager.GetEnemyCount();
    for (int i = 0; i < enemy_count; i++)
    {
        Enemy* enemy = enemy_manager.GetEnemy(i);

        // 一回当たってたら判定しない
        if (enemy->is_hit)continue;

        // 衝突処理
        DirectX::XMFLOAT3 outPosition;

        // まずは当たり判定
        if (Collision::IntersectSphereVsCylinderOut(
            nodePosition, node_radius,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition
        ))
        {
            enemy->is_hit = true;

            // ダメージを与える判定
            if (enemy->ApplyDamage(attack_power*motion_value))
            {
                HitStopON(0.15f);
                // このフレームで与えたダメージを保持
                add_damage += attack_power * motion_value;
                // もし倒したら撃破数を増やす。
                if (enemy->death)
                    kill_count++;
                if (is_counter)
                    enemy->fear_frag = true;
            }
        }


    }

}

// 入力処理
void PlayerInputComponent::Update(GameObject* gameobj, float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();

    float lx = gamePad.GetAxisLX();
    float ly = gamePad.GetAxisLY();


}


DirectX::XMFLOAT3 Player::GetMoveVec(float input_x, float input_y)
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


void Player::SkillInit()
{
    RetentionParamSet();
    for(auto& skill:skills)
    {
        skill->Init();
    }
}

void Player::SkillUpdate()
{
    for (auto& skill : skills)
    {
        skill->Update();
    }
}

void Player::SkillFin()
{
    skills.clear();
    RetentionParamGet();
}

void PlayerPhysicsComponent::Initialize(GameObject* gameobj)
{
    Player* player = dynamic_cast<Player*> (gameobj);
    player->StateMachineInitialize();

}

void PlayerPhysicsComponent::Update(GameObject* gameobj, float elapsedTime)
{
    Player* player = dynamic_cast<Player*> (gameobj);
    player->SkillUpdate();
    player->ResetAddDamage();
    player->CalcSPAttackTime();
    player->StateMachineUpdate();
    player->UpdateVelocity(elapsedTime);

}
