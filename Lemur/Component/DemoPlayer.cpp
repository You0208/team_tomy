#include "DemoPlayer.h"
#include "DemoEnemyManager.h"
#include "../Graphics/framework.h"
#include "Lemur/Graphics/Camera.h"

void DemoPlayerGraphicsComponent::Initialize(GameObject* gameobj)
{
	DemoPlayer* demoPlayer = dynamic_cast<DemoPlayer*> (gameobj);
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	demoPlayer->SetModel(ResourceManager::Instance().LoadModelResource(graphics.GetDevice(), ".\\resources\\Model\\nico.fbx"));
}

void DemoPlayerGraphicsComponent::Update(GameObject* gameobj)
{
    DemoPlayer* demoPlayer = dynamic_cast<DemoPlayer*> (gameobj);

	ImGui::Begin("Player");
    if(ImGui::TreeNode("Transform"))
    {
        DirectX::XMFLOAT3 pos = demoPlayer->GetPosition();
        ImGui::DragFloat3("position", &pos.x);
        demoPlayer->SetPosition(pos);

        ImGui::TreePop();
    }
	ImGui::End();
}

void DemoPlayerGraphicsComponent::Render(GameObject* gameobj,float elapsedTime,ID3D11PixelShader* replaced_pixel_shader)
{
	DemoPlayer* demoPlayer = dynamic_cast<DemoPlayer*> (gameobj);
	demoPlayer->Render(elapsedTime, replaced_pixel_shader);
}

// ���͏���
void DemoPlayerInputComponent::Update(GameObject* gameobj, float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();

	float lx = gamePad.GetAxisLX();
	float ly = gamePad.GetAxisLY();


    DemoPlayer* demoPlayer = dynamic_cast<DemoPlayer*> (gameobj);
    DirectX::XMFLOAT3 vec= GetMoveVec(lx, ly);
    float walk_speed = 10.0f;
    demoPlayer->Move(vec.x, vec.z, walk_speed);
}

DirectX::XMFLOAT3 DemoPlayerInputComponent::GetMoveVec(float input_x, float input_y)
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

void DemoPlayerPhysicsComponent::Initialize(GameObject* gameobj)
{
	DemoPlayer* demoPlayer = dynamic_cast<DemoPlayer*> (gameobj);
}
