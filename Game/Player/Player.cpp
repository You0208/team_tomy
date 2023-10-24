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
#include "Game/MathHelper.h"

void PlayerGraphicsComponent::Initialize(GameObject* gameobj)
{
    Player* player = dynamic_cast<Player*> (gameobj);
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    player->SetModel(ResourceManager::Instance().LoadModelResource(graphics.GetDevice(), ".\\resources\\Player\\player_v013.fbx"));

    player->GetModel()->SetupRootMotion("polySurface1", "J_root");
    /*----------------- �X�v���C�g ---------------*/
    player->spr_damage = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\number2.png");

    /*----------------- �G�t�F�N�g ---------------*/
    //player->slash = std::make_unique<Effect>("./resources/Effect/test/bigLightnigv001.efk");
    player->slash = std::make_unique<Effect>("./resources/Effect/slash.efk");
    //player->slash = std::make_unique<Effect>("./resources/Effect/test/hit/Hit.efk");

}

void PlayerGraphicsComponent::Update(GameObject* gameobj)
{
}

void PlayerGraphicsComponent::Render(GameObject* gameobj, float elapsedTime, ID3D11PixelShader* replaced_pixel_shader, bool shadow )
{
    Player* player = dynamic_cast<Player*> (gameobj);

    if (shadow)
    {
        ID3D11PixelShader* null_pixel_shader{ NULL };
        player->Render(elapsedTime, &null_pixel_shader);
    }
    else
    {
        player->Render(elapsedTime, replaced_pixel_shader);
    }
    player->DebugImgui();
}

void Player::DebugImgui()
{
    ImGui::Begin("Player");
    ImGui::DragFloat("skill_ui_offset_y", &skill_ui_offset_y);
    ImGui::DragFloat("skill_ui_scale", &skill_ui_scale,0.1);
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

        ImGui::InputInt("total_point", &total_point);
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
        ImGui::DragFloat("motion_value", &motion_value);
        ImGui::Checkbox("attack_collision_flag", &attack_collision_flag);
        ImGui::Checkbox("is_hit_stop", &is_hit_stop);
        ImGui::Checkbox("invincible", &invincible);
        ImGui::DragInt("invincible_frame", &invincible_frame);
        ImGui::Checkbox("can_SP_attack", &can_SP_attack);
        ImGui::DragFloat("SP_attack_cool_time_ms", &SP_attack_cool_time_ms);
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

    // ���G���͐ŕ`��
    if (invincible)
        color = { 0.0f,0.0f,1.0f,0.0f };

    debug_renderer->DrawCylinder(position, radius, height, color);

    if(attack_collision_flag)
    {
        DirectX::XMFLOAT3 position = Model->joint_position("polySurface1", "J_root", &keyframe, world);
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

    // �i�s�x�N�g�����[���x�N�g���Ȃ�͓��͂���ĂȂ�
    if (vec.x == 0.0f && vec.z == 0.0f)
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
        can_SP_attack = false;
        SP_attack_cool_timer_ms += high_resolution_timer::Instance().time_interval();
    }
}

bool Player::CounterJudge(DirectX::XMFLOAT3 hit_pos)
{
    if (!can_counter)return false;

    // ��]�s��̍쐬
    DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    // �O�����x�N�g���擾
    DirectX::XMVECTOR Forward = rotation_matrix.r[2];

    //�v���C���[�̈ʒu
    DirectX::XMVECTOR Player_pos = DirectX::XMLoadFloat3(&position);
    // �G�̈ʒu
    DirectX::XMVECTOR Enemy_pos = DirectX::XMLoadFloat3(&hit_pos);
    // �v���C���[����G�܂ł̃x�N�g��
    DirectX::XMVECTOR Player_to_Enemy = DirectX::XMVectorSubtract(Enemy_pos, Player_pos);

    // ���K��
    Forward = DirectX::XMVector3Normalize(Forward);
    Player_to_Enemy = DirectX::XMVector3Normalize(Player_to_Enemy);

    // ���ςŊp�x�擾
    float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(Forward, Player_to_Enemy));

    if (dot > 0) 
    {
        is_counter = true;
        return true;
    }
    return false;
}

void Player::AttackAngleInterpolation()
{
    int enemy_count = EnemyManager::Instance().GetEnemyCount();
    if (enemy_count <= 0)return;

    // �G�Ǝ����Ƃ̋���
    float player_to_enemy = FLT_MAX;
    DirectX::XMFLOAT3 enemy_pos;

    // ���b�N�I�����Ă��烍�b�N�I�������G�Ɍ����悤�ɕ␳
    if (Camera::Instance().GetLockONEnemy())enemy_pos = Camera::Instance().GetLockONEnemy()->GetPosition();

    // ���ĂȂ��������ԋ߂���Ɍ����␳
    else
    {
        // �S�G�l�~�[�Ƃ̋�������
        for (auto enemy : EnemyManager::Instance().GetEnemies())
        {
            float length = Length(position, enemy->GetPosition());

            if (player_to_enemy > length)
            {
                enemy_pos = enemy->GetPosition();
                player_to_enemy = length;
            }
        }

        // ��������A���ꂷ���Ă���␳���Ȃ�
        if (player_to_enemy > can_attack_interpolation_length)return;
    }

    // todo Nero �������牺�����W���[�����Benemy_pos�͈����ɂ��邩
    // ���̒l�����ς��������������Ԃł��Ȃ�
    float theta = 0.7f;
    DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR Enemy_pos = DirectX::XMLoadFloat3(&enemy_pos);

    // �v���C���[����G�̈ʒu
    DirectX::XMVECTOR Pos_To_EnemyPos = DirectX::XMVectorSubtract(Enemy_pos, Pos);
    Pos_To_EnemyPos = DirectX::XMVector3Normalize(Pos_To_EnemyPos);

    // ��]�s��̍쐬
    DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    // �O�����x�N�g���擾
    DirectX::XMVECTOR Forward = rotation_matrix.r[2];

    // ���ϔ���
    if(CalcAngle(Forward,Pos_To_EnemyPos,theta))
    {
        /*-------- ����N���A�������� --------*/


        // �v���C���[����G�̈ʒu
        DirectX::XMFLOAT3 pos_to_enemy_pos;
        DirectX::XMStoreFloat3(&pos_to_enemy_pos, Pos_To_EnemyPos);


        // �v���C���[�̑O����
        float front_x = sinf(rotation.y);
        float front_z = cosf(rotation.y);

        float dot = (pos_to_enemy_pos.x * front_x) + (pos_to_enemy_pos.z * front_z);

        float angle = std::acosf(dot);

        float cross = (pos_to_enemy_pos.x * front_z) - (pos_to_enemy_pos.z * front_x);
        if(cross<0.0f)
        {
            rotation.y -= angle;
        }
        else
        {
            rotation.y += angle;
        }
    }
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

    // �X�L����m������΃x�b�g��������߂�
    if (HaveSkill("Schemer"))
    {
        max_health += bet_MHP;
        attack_power += bet_AP;
        speed_power += bet_SP;
    }

}

void Player::CollisionNodeVsEnemies(const char* mesh_name,const char* bone_name, float node_radius)
{

    // �v���C���[���̃m�[�h�ʒu�擾
    DirectX::XMFLOAT3 nodePosition = Model->joint_position(mesh_name, bone_name, &keyframe, world);

    // �w��̃m�[�h�ƑS�Ă̓G�𑍓�����ŏՓˏ���

    EnemyManager& enemy_manager = EnemyManager::Instance();
    int enemy_count = enemy_manager.GetEnemyCount();
    for (int i = 0; i < enemy_count; i++)
    {
        Enemy* enemy = enemy_manager.GetEnemy(i);

        // ��񓖂����Ă��画�肵�Ȃ�
        if (enemy->is_hit)continue;

        for (auto& node : enemy->hit_collisions)
        {
            // �G�̋�炢�����蔻��
            DirectX::XMFLOAT3 node_pos = enemy->GetModel()->joint_position(enemy->meshName.c_str(), node->bone_name.c_str(), &enemy->keyframe, enemy->world);

            // �܂��͓����蔻��
            if (Collision::IntersectSphereVsSphere(
                nodePosition, node_radius,
                node_pos,
                node->node_radius
            ))
            {
                // ���i�q�b�g�h�~�p
                enemy->is_hit = true;

                // �܂�����Ԃ���Ȃ����������Ԃɂ���
                if (!enemy->belligerency)enemy->belligerency = true;

                // �_���[�W��^���锻��
                if (enemy->ApplyDamage(attack_power * motion_value))
                {
                    // �q�b�g�X�g�b�v
                    HitStopON(0.15f);
                    ////�G�t�F�N�g
                    //slash->Play(position, 1.0f);
                    // todo ���ꂢ��H
                    //��ʐU��
                    Camera::Instance().ScreenVibrate(0.05f, 0.3f);
                    // �_���[�W�ʕ`��
                    enemy->DamageRenderSet(attack_power * motion_value,node_pos);

                    // ���̃t���[���ŗ^�����_���[�W��ێ�
                    add_damage += attack_power * motion_value;

                    // �����|�����猂�j���𑝂₷�B
                    if (enemy->death)
                        kill_count++;

                    if (is_counter)
                        enemy->fear_frag = true;

                    break;
                }
                
            }
        }

    }

}

void Player::UIRender()
{
    SkillUIRender();

}

// ���͏���
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

    // �J���������ƃX�e�C�b�N�̓��͒l�ɂ���Đi�s�������v�Z����B

    /*--------------- �J���������̌v�Z ---------------*/

    Camera& camera = Camera::Instance();

    DirectX::XMMATRIX V = camera.GetViewMatrix();
    DirectX::XMMATRIX W = DirectX::XMMatrixInverse(nullptr, V);
    DirectX::XMFLOAT4X4 w;
    DirectX::XMStoreFloat4x4(&w, W);

    const DirectX::XMFLOAT3& cameraRight = { w._11,w._12,w._13 };
    const DirectX::XMFLOAT3& cameraFront = { w._31,w._32,w._33 };

    // �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���B

    // �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ��B
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    const float cameraRightLength = sqrtf(
        (cameraRight.x * cameraRight.x) + cameraRight.z * cameraRight.z
    );
    if (cameraRightLength > 0.0f)
    {
        // �P�ʃx�N�g����(���K��)
        cameraRightX = cameraRightX / cameraRightLength;
        cameraRightZ = cameraRightZ / cameraRightLength;
    }

    //�J�����O�����̃x�N�g����XZ�P�ʃx�N�g���ɕϊ��B
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(
        (cameraFront.x * cameraFront.x) + cameraFront.z * cameraFront.z
    );
    if (cameraFrontLength > 0.0f)
    {
        //�P�ʃx�N�g����(���K��)
        cameraFrontX = cameraFrontX / cameraFrontLength;
        cameraFrontZ = cameraFrontZ / cameraFrontLength;
    }

    // �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
    //�X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
    //�i�s�x�N�g�����v�Z����B
    DirectX::XMFLOAT3 vec{};
    vec.x = cameraFrontX * sinf(input_y) + cameraRightX * sinf(input_x);
    vec.z = cameraFrontZ * sinf(input_y) + cameraRightZ * sinf(input_x);
    //y�������ɂ͈ړ����Ȃ�
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
    for (auto& skill : skills)
    {
        skill->Fin();
    }

    RetentionParamGet();
    skills.clear();
}

void Player::SkillUIRender()
{
    
    ID3D11DeviceContext* dc = Lemur::Graphics::Graphics::Instance().GetDeviceContext();
    for (int skill_i = 0; skill_i < skills.size(); skill_i++)
    {
        skills.at(skill_i)->UI_spr->render(dc, 421*skill_ui_scale * skill_i, skill_ui_offset_y, 421 * skill_ui_scale, 402 * skill_ui_scale);
    }

}

void Player::DamageRender(DirectX::XMFLOAT3 hit_pos)
{

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
