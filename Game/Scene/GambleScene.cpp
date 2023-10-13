#include "GambleScene.h"

#include "Lemur/Input/Input.h"
#include "Lemur/Scene/SceneManager.h"

void GambleScene::Initialize()
{
    // アセットロード
}

void GambleScene::Finalize()
{

}

void GambleScene::Update(HWND hwnd, float elapsedTime)
{
    GamePad& game_pad = Input::Instance().GetGamePad();
    if (game_pad.GetButtonDown() & GamePad::BTN_START)
    {
        //Lemur::Scene::SceneManager::Instance().ChangeScene()
    }
}

void GambleScene::Render(float elapsedTime)
{
}
