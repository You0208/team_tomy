#include "TitleScene.h"

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
        //Lemur::Scene::SceneManager::Instance().ChangeScene(// Todo@‚±‚±‚Ç‚¤‚·‚é)
    }
}

void TitleScene::Render(float elapsedTime)
{

    //title->render()
}
