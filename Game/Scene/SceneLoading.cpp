#include "SceneLoading.h"
#include "Lemur/Scene/SceneManager.h"
#include "Lemur/Graphics/Camera.h"
#include "high_resolution_timer.h"
#include "imgui.h"

void LoadingScene::Initialize()
{
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    SetState();
    // シェーダー関連
    // スレッド
    {
       // thread = new std::thread(LoadingThread, this);
    }
    // ゲーム部分
    {
        spr_icon = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\Load_icon.png");
        spr_back = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\Loading_screen.png");
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
    //// 次のシーンの準備が完了したらシーンを切り替える
    //if (nextScene->IsReady())
    //{
    //    Lemur::Scene::SceneManager::Instance().ChangeScene(nextScene);
    //    nextScene = nullptr;
    //}
}

void LoadingScene::Render(float elapsedTime)
{
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

    SetUpRendering();
    // 2D描画
    {
        // sprite描画
        {
            immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
            immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
            immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);

            spr_back->render(immediate_context,  0,0 ,  SCREEN_WIDTH, SCREEN_HEIGHT, 1, 1, 1, 1, 0);
            spr_icon->animation(immediate_context, {1691,835}, { 150, 150 }, { 1, 1, 1, 1 }, 0, { 110, 110 });
        }
    }
}

void LoadingScene::LoadingThread(LoadingScene* scene)
{
    // COM関連の初期化でスレッド毎に呼ぶ必要がある
    CoInitialize(nullptr);

    // 次のシーンの初期を行う
    scene->nextScene->Initialize();

    // スレッドが終わる前にCOM関連の終了化
    CoUninitialize();

    // 次のシーンの準備完了設定
    scene->nextScene->SetReady();
}
