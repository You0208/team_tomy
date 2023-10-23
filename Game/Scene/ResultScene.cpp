#include "Game/Scene/ResultScene.h"

#include "GambleScene.h"
#include "Game/Manager/CharacterManager.h"
#include "Lemur/Input/Input.h"
#include "Lemur/Scene/SceneManager.h"

void ResultScene::Initialize()
{
    // ここで絵の設定
    player = CharacterManager::Instance().GetPlayer();

    // クリアしてたら報酬を与える
    if (clear)
        GiveReward();
}

void ResultScene::Finalize()
{
}

void ResultScene::Update(HWND hwnd, float elapsedTime)
{
    // todo ここの見た目の演出どうする？
    Mouse& mouse = Input::Instance().GetMouse();
    if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
        Lemur::Scene::SceneManager::Instance().ChangeScene(new GambleScene);


}

void ResultScene::Render(float elapsedTime)
{
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

    SetUpRendering();

    player->DebugImgui();
    DebugImGui();

}

void ResultScene::DebugImGui()
{

}

void ResultScene::GiveReward()
{
    player->attack_power  += player->bet_AP * bet_rate;
    player->defense_power += player->bet_DP * bet_rate;
    player->speed_power   += player->bet_SP * bet_rate;
    player->max_health    += static_cast<int>(player->bet_MHP * bet_rate);
}



// todo エフェクト
// todo ルートモーション
// todo ギャンブルシーンのベットレートらへん
// todo スキル
// todo UIのずれ
// todo テクスチャ
// todo カメラワーク

// todo あとなんかいろいろ