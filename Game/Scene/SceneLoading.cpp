#include "SceneLoading.h"
#include "Lemur/Scene/SceneManager.h"
#include "Lemur/Graphics/Camera.h"

void LoadingScene::Initialize()
{
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    // �V�F�[�_�[�֘A
    {
        SetState();
        HRESULT hr{ S_OK };
        D3D11_BUFFER_DESC buffer_desc{};
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;// �ǂݎ�肨��я������݂��ǂ̂悤�ɍs���邩
        buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;// �o�b�t�@���p�C�v���C���ɂǂ̂悤�Ƀo�C���h(���т�)����邩�����
        buffer_desc.CPUAccessFlags = 0;// CPU �A�N�Z�X �t���O�iCPU �A�N�Z�X���K�v�Ȃ��ꍇ�� 0�j
        buffer_desc.MiscFlags = 0;// ���̑��̃t���O�i���g�p�ɏꍇ��0�j
        buffer_desc.StructureByteStride = 0;//�o�b�t�@���\�����o�b�t�@��\���ꍇ�́A�o�b�t�@�\�����̊e�v�f�̃T�C�Y
        {
            buffer_desc.ByteWidth = sizeof(scene_constants);// �o�b�t�@�̃T�C�Y 
            hr = graphics.GetDevice()->CreateBuffer(&buffer_desc, nullptr, constant_buffers.GetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        }
    }
    // �X���b�h
    {
        thread = new std::thread(LoadingThread, this);
    }
    // �Q�[������
    {

    }
}

void LoadingScene::Finalize()
{
}

void LoadingScene::Update(HWND hwnd, float elapsedTime)
{

    // ���̃V�[���̏���������������V�[����؂�ւ���
    if (nextScene->IsReady())
    {
        Lemur::Scene::SceneManager::Instance().ChangeScene(nextScene);
        nextScene = nullptr;
    }
}

void LoadingScene::Render(float elapsedTime)
{
    Camera& camera = Camera::Instance();
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
    ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

    camera.SetPerspectiveFov(immediate_context);
    SetUpRendering();

    immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);
    // �[�x�X�e���V���X�e�[�g�̐ݒ�
    immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].Get(), 0);
    immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::SOLID)].Get());

    immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);
    immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].Get(), 0);
    immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::SOLID)].Get());
    {
        D3D11_VIEWPORT viewport;
        UINT num_viewports{ 1 };
        immediate_context->RSGetViewports(&num_viewports, &viewport);

        float aspect_ratio{ viewport.Width / viewport.Height };
        DirectX::XMMATRIX P{ DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30), aspect_ratio, 0.1f, 1000.0f) };

        DirectX::XMVECTOR eye = camera.GetEye();
        DirectX::XMVECTOR focus = camera.GetFocus();
        DirectX::XMVECTOR up{ DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) };
        DirectX::XMMATRIX V{ DirectX::XMMatrixLookAtLH(eye, focus, up) };

        DirectX::XMStoreFloat4x4(&scene_constants.view_projection, V * P);
        // UNIT.16
        scene_constants.camera_position = camera_position;
    }

    // �萔�o�b�t�@�̍X�V
    {
        immediate_context->UpdateSubresource(constant_buffers.Get(), 0, 0, &scene_constants, 0, 0);
        immediate_context->VSSetConstantBuffers(1, 1, constant_buffers.GetAddressOf());
        immediate_context->PSSetConstantBuffers(1, 1, constant_buffers.GetAddressOf());
    }

    // 3D�`��
    {
        immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].Get(), 0);
        immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
        {
            const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
                { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
                { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
                { -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
                { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
            };
#if 1
            float scale_factor = 1.0f; // To change the units from centimeters to meters, set 'scale_factor' to 0.01.
#else
            const float scale_factor = 0.01f; // To change the units from centimeters to meters, set 'scale_factor' to 0.01.
#endif
            DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0])* DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };
            DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scaling.x, scaling.y, scaling.z) };
            DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) };
            DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z) };
            DirectX::XMFLOAT4X4 world;
            DirectX::XMStoreFloat4x4(&world, C * S * R * T);
        }
    }

    // 2D�`��
    {
        // sprite�`��
        {
            immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
            immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
            immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);

            immediate_context->IASetInputLayout(sprite_input_layout.Get());
            immediate_context->VSSetShader(sprite_vertex_shader.Get(), nullptr, 0);
            immediate_context->PSSetShader(sprite_pixel_shader.Get(), nullptr, 0);
            immediate_context->PSSetSamplers(0, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());
            immediate_context->PSSetShaderResources(1, 1, mask_texture.GetAddressOf());

        }
    }
}

void LoadingScene::LoadingThread(LoadingScene* scene)
{
    // COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    CoInitialize(nullptr);

    // ���̃V�[���̏������s��
    scene->nextScene->Initialize();

    // �X���b�h���I���O��COM�֘A�̏I����
    CoUninitialize();

    // ���̃V�[���̏��������ݒ�
    scene->nextScene->SetReady();
}
