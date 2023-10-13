#include "TitleScene.h"

#include "GambleScene.h"
#include "Lemur/Input/Input.h"
#include "Lemur/Scene/SceneManager.h"

void TitleScene::Initialize()
{
    title = std::make_unique<sprite>(Lemur::Graphics::Graphics::Instance().GetDevice(), L"./resources/Image/pic001.png");
}

void TitleScene::Finalize()
{
}

void TitleScene::Update(HWND hwnd, float elapsedTime)
{
    GamePad& game_pad = Input::Instance().GetGamePad();
    if(game_pad.GetButtonDown()&GamePad::BTN_START)
    {
        Lemur::Scene::SceneManager::Instance().ChangeScene(new GambleScene);
    }
}

void TitleScene::Render(float elapsedTime)
{
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

    SetUpRendering();

    // 2D•`‰æ
    {
        if (title)
        {
            immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
            immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
            immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);

            title->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        }
    }
}
