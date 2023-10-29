#include "TitleScene.h"

#include "GambleScene.h"
#include "Lemur/Input/Input.h"
#include "Lemur/Scene/SceneManager.h"
#include "imgui.h"
#include "Game/Manager/CharacterManager.h"
#include "Lemur/Audio/AudioManager.h"
extern int wave_count;

bool tutorial;
void TitleScene::Initialize()
{
    step = 0;

    title = std::make_unique<sprite>(Lemur::Graphics::Graphics::Instance().GetDevice(), L"./resources/Image/titile.png");
    spider_anim = std::make_unique<sprite>(Lemur::Graphics::Graphics::Instance().GetDevice(), L"./resources/Image/spider_anime.png");
    wave_count = 1;
    Player* player = CharacterManager::Instance().GetPlayer();

    if (player) 
    {
        player->Delete();
        delete player;
    }
    Lemur::Audio::AudioManager::Instance().play_bgm(Lemur::Audio::BGM::TITLE, true);
}

void TitleScene::Finalize()
{
    Lemur::Audio::AudioManager::Instance().stop_BGM(Lemur::Audio::BGM::TITLE);
}

void TitleScene::Update(HWND hwnd, float elapsedTime)
{
    GamePad& game_pad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    switch (step)
    {
    case 0:
        if (game_pad.GetButtonDown() & GamePad::BTN_A ||
            mouse.GetButtonDown() & Mouse::BTN_RIGHT)
        {
            // todo ここで演出
            tutorial = true;

            step++;
        }

        if (game_pad.GetButtonDown() & GamePad::BTN_START ||
            mouse.GetButtonDown() & Mouse::BTN_MIDDLE)
        {
            tutorial = false;

            // todo ここで演出

            step++;
        }


        break;

    case 1:

        // タイトルから行くときは初めから。
        wave_count = 1;
        Lemur::Audio::AudioManager::Instance().play_se(Lemur::Audio::SE::SENI, false);

        Lemur::Scene::SceneManager::Instance().ChangeScene(new GambleScene);



        break;
    }

    anim_timer += high_resolution_timer::Instance().time_interval();

    // todo なんかうまくいかん
    tex_pos_x = ((static_cast<int>(anim_timer) / 1) % 3) * 300;
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
            spider_anim->render(immediate_context, spider_pos.x, spider_pos.y, 1200, 300, 1, 1, 1, 1, 0.0f, tex_pos_x, 0, 300, 300);
        }
    }

    DrawImGui();
}

void TitleScene::DrawImGui()
{
  
}
