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
        // ��퓬
        ai_tree->AddNode("Root", "NonBattle", 1, BehaviorTree::SelectRule::Priority, nullptr, nullptr);
        {
            // �p�j
            ai_tree->AddNode("NonBattle", "Wander", 1, BehaviorTree::SelectRule::Non, nullptr, new WanderAction(this));
            //todo �ҋ@

        }
        // �퓬
        ai_tree->AddNode("Root","Battle",0,BehaviorTree::SelectRule::Priority,,nullptr)
        // todo �O��������ǐՂ���J�Í�
    }


}

void Enemy::Turn(float vx, float vz, float speed)
{
    //// todo ���Ԏ���Ă����悤��SceneManager�ɓ����B
    //speed *= GameScene::timer;

    //// �x�N�g���̑傫�����擾
    //float Length = sqrtf(vx * vx + vz * vz);

    //// �x�N�g���̑傫����0�Ȃ�(�[���x�N�g���Ȃ�)
    //if (Length <= 0.01)
    //{
    //    return;
    //}

    //// �i�s�x�N�g���̐��K��
    //vx = vx / Length;
    //vz = vz / Length;

    //// ���g�̉�]�l����O���������߂�B
    //float frontX = sinf(angle.y);
    //float frontZ = cosf(angle.y);

    //// ��]�p�����߂邽�߂ɁA2�̒P�ʃx�N�g���̓��ς��v�Z����
    //float dot = (vx * frontX) + (vz * frontZ);

    //// dot �� -1.0f �` 1.0f �ɂȂ�B�Ȃ̂� rot �� 0.0f �` 2.0f �ɂȂ�B
    //float rot = 1.0f - dot;

    //// ���ς��������Ȃ�����
    //if (rot < speed) speed = rot; // ���̕�������ς���p�x������������

    //// ���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
    //float cross = (vx * frontZ) - (vz * frontX);

    //// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���]���s����
    //// ���E������s�����Ƃɂ���č��E��]��I������
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
    // �v���C���[�̈ʒu
    DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();

    // x������
    float vec_x = player_pos.x - position.x;
    // z������
    float vec_z = player_pos.z - position.z;
    // ���v����(���)
    float length_sq = (vec_x * vec_x) + (vec_z * vec_z);

    // �������͈͈ȓ��Ȃ�
    if (length_sq < found_distance * found_distance)
    {
        //���͐��ʔ���

        // �O����X�x�N�g��
        float front_x = sinf(rotation.y);
        // �O����Z�x�N�g��
        float front_z = cosf(rotation.y);

        // �����O��
        float length = sqrtf(length_sq);
        // ���K��
        vec_x /= length;
        vec_z /= length;

        // ����
        float dot = (front_x * vec_x) + (front_z * vec_z);
        // ���ʂɋ�����
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

    //// ���K��
    //vx /= dist;
    //vz /= dist;

    //Move(vx, vz, base_move_speed * move_speed_rate);
    //Turn(vx, vz, turnSpeed * turn_speed_rate);

}

void Enemy::SetRandomTargetPosition()
{
    //// territory_range�̑傫���̉~�Ń����_���Ȓl���Ƃ�B
    //float theta = Argent::Random::Generate(-DirectX::XM_PI, DirectX::XM_PI);
    //float range = Argent::Random::Generate(0.0f, territory_range);

    //target_position.x = territory_origin.x + sinf(theta) * range;
    //target_position.y = territory_origin.y;
    //target_position.z = territory_origin.z + cosf(theta) * range;

}

// ���͏���
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

void EnemyPhysicsComponent::Initialize(GameObject* gameobj)
{
    Enemy* enemy = dynamic_cast<Enemy*> (gameobj);

    //enemy->

}
