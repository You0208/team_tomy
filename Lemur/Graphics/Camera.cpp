#include "Camera.h"
#include"..\Input/Input.h"
#include "Game/Manager/CharacterManager.h"

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

    /*----------- �R���g���[���[���� ------------*/
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();

    float lx = gamePad.GetAxisLX();
    float ly = gamePad.GetAxisLY();

    // �J�����̉�]���x
    float speed = rollSpeed * elapsedTime;

    // �X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
    angle.x -= ay * speed;
    angle.y += ax * speed;

#if 0
    /*------------- �}�E�X���� -------------*/
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
# endif
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
#if 0
    SetCursorPos(1920 / 2, 1080 / 2);
    mouse.SetPositionX(1920 / 2);
    mouse.SetPositionY(1080 / 2);
#endif

    if(CharacterManager::Instance().GetPlayer()!=nullptr)target = CharacterManager::Instance().GetPlayer()->GetPosition();

    // �J�����̉�]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // ��]�s�񂩂�O�����Ƀx�N�g�����o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    DirectX::XMFLOAT3 eye = {};
    eye.x = target.x - front.x * range;// front=�����A�������|���Z���邱�ƂŒ������킩��
    eye.y = target.y - front.y * range;// �ڕW�n�_����J�����̋������������ƂŃJ�����̈ʒu���o��
    eye.z = target.z - front.z * range;// ����W�y�[�W
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

void Camera::DrawDebug()
{
}

void Camera::UpdateDebug()
{
}
