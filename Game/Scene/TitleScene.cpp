#include "TitleScene.h"

#include "GambleScene.h"
#include "Lemur/Input/Input.h"
#include "Lemur/Scene/SceneManager.h"
#include "imgui.h"
extern int wave_count;

void TitleScene::Initialize()
{
    title = std::make_unique<sprite>(Lemur::Graphics::Graphics::Instance().GetDevice(), L"./resources/Image/titile.png");
    spider_anim = std::make_unique<sprite>(Lemur::Graphics::Graphics::Instance().GetDevice(), L"./resources/Image/spider_anime.png");
}

void TitleScene::Finalize()
{
}

void TitleScene::Update(HWND hwnd, float elapsedTime)
{
    GamePad& game_pad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();
    if(game_pad.GetButtonDown()&GamePad::BTN_START||
        mouse.GetButtonDown()&Mouse::BTN_RIGHT)
    {
        // タイトルから行くときは初めから。
        wave_count = 1;

        Lemur::Scene::SceneManager::Instance().ChangeScene(new GambleScene);
    }

    anim_timer += high_resolution_timer::Instance().time_interval();

    tex_pos_x = (static_cast<int>(anim_timer) / 12) % 3;
}

void TitleScene::Render(float elapsedTime)
{
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

    SetUpRendering();

    // 2D描画
    {
        if (title)
        {
            immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
            immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
            immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);

            title->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            spider_anim->render(immediate_context, spider_pos.x, spider_pos.y, 300, 300, 1, 1, 1, 1, 0.0f, tex_pos_x, 0, 300, 300);
        }
    }

    DrawImGui();
}

void TitleScene::DrawImGui()
{
    ImGui::Begin("scene");
    ImGui::DragFloat2("spider_pos", &spider_pos.x);
    ImGui::DragInt("tex_pos_x", &tex_pos_x);
    ImGui::End();
}
