#include "SceneLoading.h"
#include "Lemur/Scene/SceneManager.h"
#include "Lemur/Graphics/Camera.h"
#include "high_resolution_timer.h"
#include "imgui.h"

void LoadingScene::Initialize()
{
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    SetState();
    // �V�F�[�_�[�֘A
    // �X���b�h
    {
        thread = new std::thread(LoadingThread, this);
    }
    // �Q�[������
    {
        spr_icon = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\Load_icon.png");
    }
}

void LoadingScene::Finalize()
{
}

void LoadingScene::Update(HWND hwnd, float elapsedTime)
{
    ImGui::Begin("Shader");

    time=high_resolution_timer::Instance().time_stamp();
    ImGui::SliderFloat2("poo", &poo.x,0,1920);
    ImGui::End();
    // ���̃V�[���̏���������������V�[����؂�ւ���
    if (nextScene->IsReady())
    {
        Lemur::Scene::SceneManager::Instance().ChangeScene(nextScene);
        nextScene = nullptr;
    }
}

void LoadingScene::Render(float elapsedTime)
{
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

    SetUpRendering();
    // 2D�`��
    {
        // sprite�`��
        {
            immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
            immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
            immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);

            spr_icon->animation(immediate_context, poo, { 150, 150 }, { 1, 1, 1, 1 }, 0, { 110, 110 });
            //spr_icon->render(immediate_context,  100,100 ,  110, 110 , 1, 1, 1, 1, 0);
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
