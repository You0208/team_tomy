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
    player->SetModel(ResourceManager::Instance().LoadModelResource(graphics.GetDevice(), ".\\resources\\Player\\player_v007.fbx"));
}

void PlayerGraphicsComponent::Update(GameObject* gameobj)
{
    Player* player = dynamic_cast<Player*> (gameobj);



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

        ImGui::DragInt("MaxHealth", &maxHealth);
        ImGui::DragInt("health", &health);
        ImGui::DragFloat("attack_power", &attack_power);
        ImGui::DragFloat("defense_power", &defense_power);
        ImGui::DragFloat("speed_power", &speed_power);

        ImGui::TreePop();
    }
    if(ImGui::TreeNode("Skills"))
    {
        for(auto& skill:skills)
        {
            ImGui::Text(skill->GetName().c_str());
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
        ImGui::InputInt("frame_index", &frame_index);
        ImGui::TreePop();
    }
    ImGui::End();
}

void Player::DrawDebugPrimitive()
{
    DebugRenderer* debug_renderer = Lemur::Graphics::Graphics::Instance().GetDebugRenderer();
    debug_renderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f));

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
                add_damage += attack_power * motion_value - enemy->defense_power;
                // もし倒したら撃破数を増やす。
                if (enemy->death)
                    kill_count++;
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
}

void PlayerPhysicsComponent::Initialize(GameObject* gameobj)
{
    Player* player = dynamic_cast<Player*> (gameobj);
    player->StateMachineInitialize();

    player->SkillInit();
}

void PlayerPhysicsComponent::Update(GameObject* gameobj, float elapsedTime)
{
    Player* player = dynamic_cast<Player*> (gameobj);
    player->SkillUpdate();
    player->StateMachineUpdate();
    player->UpdateVelocity(elapsedTime);

}
