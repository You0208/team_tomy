#include "Camera.h"
#include"..\Input/Input.h"
#include "Game/MathHelper.h"
#include "Game/Manager/EnemyManager.h"
#include "imgui.h"

void Camera::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
    //���_�A�����_�A���������r���[�s����쐬
    Eye = DirectX::XMLoadFloat3(&eye);
    Focus = DirectX::XMLoadFloat3(&focus);
    Up = DirectX::XMLoadFloat3(&up);
    DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);

    // �r���[�s����t�s�񉻂��A���[���h�s��ɖ߂�
    DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, World);

    // �J�������������o��
    this->right.x = world._11;
    this->right.y = world._12;
    this->right.z = world._13;

    this->up.x = world._21;
    this->up.y = world._22;
    this->up.z = world._23;

    this->front.x = world._31;
    this->front.y = world._32;
    this->front.z = world._33;

}

void Camera::SetPerspectiveFov(Microsoft::WRL::ComPtr<ID3D11DeviceContext> dc)
{
    D3D11_VIEWPORT viewport{};
    UINT num_viewports{ 1 };
    dc->RSGetViewports(&num_viewports, &viewport);

    float aspect_ratio{ viewport.Width / viewport.Height };
    // �ˉe�s��𐶐�
    P = { DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30), aspect_ratio, 0.1f, 1000.0f) };

    // �����s��𐶐�
    //DirectX::XMVECTOR eye{ DirectX::XMLoadFloat4(&camera_position) };
    V = { DirectX::XMMatrixLookAtLH(Eye, Focus, Up) };
}

void Camera::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_LEFT_SHOULDER)
    {
        InputLockOn();
    }


    if (is_lockOn) LockOnUpdate(elapsedTime);

    else NonLockOnUpdate(elapsedTime);



}

void Camera::DrawDebug()
{
    ImGui::Begin("Camera");
    if(ImGui::TreeNode("Transform"))
    {
        ImGui::DragFloat3("eye", &eye.x);
        ImGui::DragFloat3("target", &target.x);
        ImGui::DragFloat("eye_y_offset", &eye_y_offset);
        ImGui::TreePop();
    }
        ImGui::End();
}

void Camera::UpdateDebug()
{
}

void Camera::InputLockOn()
{
    // �܂����b�N�I�����ĂȂ������炷��
    if (!is_lockOn) {
        DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();

        // �G�Ǝ����Ƃ̋���
        float player_to_enemy = FLT_MAX;

        // �S�G�l�~�[�Ƃ̋�������
        for (auto enemy : EnemyManager::Instance().GetEnemies())
        {
            DirectX::XMFLOAT3 enemy_pos = enemy->GetPosition();

            float length = Length(player_pos, enemy_pos);
            if (player_to_enemy > length)
            {
                is_lockOn = true;

                player_to_enemy = length;
                lock_on_enemy = enemy;
            }
        }
    }
    //���b�N�I�����Ă���O��
    else
        is_lockOn = false;

}

void Camera::LockOnUpdate(float elapsedTime)
{
    DirectX::XMFLOAT3 enemy_pos = lock_on_enemy->GetPosition();
    DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();

    float length = Length(player_pos, enemy_pos);

    // �G����v���C���[�ւ̃x�N�g��
    DirectX::XMFLOAT3 vec =
    {
         player_pos.x - enemy_pos.x ,
         player_pos.y - enemy_pos.y ,
         player_pos.z - enemy_pos.z ,
    };
    vec.x = vec.x / length;
    vec.y = vec.y / length;
    vec.z = vec.z / length;


    eye={
        player_pos.x + (vec.x * range),
        player_pos.y + (vec.y * range) + eye_y_offset,
        player_pos.z + (vec.z * range),
    };

    target = lock_on_enemy->GetPosition();

    //�J�����̎��_�ƒ����_��ݒ�
    SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));

}

void Camera::NonLockOnUpdate(float elapsedTime)
{
    GamePad& game_pad = Input::Instance().GetGamePad();

    /*----------- �R���g���[���[���� ------------*/
    float ax = game_pad.GetAxisRX();
    float ay = game_pad.GetAxisRY();

    float lx = game_pad.GetAxisLX();
    float ly = game_pad.GetAxisLY();

    // �J�����̉�]���x
    float speed = rollSpeed * elapsedTime;

    // �X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
    angle.x -= ay * speed;
    angle.y += ax * speed;

#if 0 /*------------- �}�E�X���� -------------*/
    Mouse& mouse = Input::Instance().GetMouse();

    float mouse_pos_x = static_cast<int> (mouse.GetPositionX() - mouse.GetOldPositionX());
    float mouse_pos_y = static_cast<int> (mouse.GetPositionY() - mouse.GetOldPositionY());

    // �}�E�X�ł̊��x�̕␳�l�ArollSpeed�ɂ�����B
    float speed_fit = 1.0f;

    // �J�����̉�]���x�B
    speed = speed_fit * rollSpeed * elapsedTime;


    // �X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]�B
    angle.x += mouse_pos_y * speed;
    angle.y += mouse_pos_x * speed;

    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
#if 0
    SetCursorPos(1920 / 2, 1080 / 2);
    mouse.SetPositionX(1920 / 2);
    mouse.SetPositionY(1080 / 2);
#endif
#endif

    target = CharacterManager::Instance().GetPlayer()->GetPosition();
    // �J�����̉�]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // ��]�s�񂩂�O�����Ƀx�N�g�����o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    eye.x = target.x - (front.x * range);               // front=�����A�������|���Z���邱�ƂŒ������킩��
    eye.y = target.y - (front.y * range) + eye_y_offset;// �ڕW�n�_����J�����̋������������ƂŃJ�����̈ʒu���o��
    eye.z = target.z - (front.z * range);               // ����W�y�[�W
    // X���̃J������]�𐧌�
    angle.x = min(angle.x, maxAngleX);
    angle.x = max(angle.x, minAngleX);
    //�͔͉𓚁�
    if (angle.x < minAngleX)
    {
        angle.x = minAngleX;
    }
    if (angle.x > maxAngleX)
    {
        angle.x = maxAngleX;
    }

    // Y���̉�]�l��-3.14~3.14�Ɏ��܂�悤�ɂ���
    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > -DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }

    //�J�����̎��_�ƒ����_��ݒ�
    SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));

}
