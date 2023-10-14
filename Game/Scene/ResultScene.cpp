#include "Game/Scene/ResultScene.h"

#include "GambleScene.h"
#include "Lemur/Input/Input.h"
#include "Lemur/Scene/SceneManager.h"

void ResultScene::Initialize()
{
    // �����ŊG�̐ݒ�

}

void ResultScene::Finalize()
{
}

void ResultScene::Update(HWND hwnd, float elapsedTime)
{
    Mouse& mouse = Input::Instance().GetMouse();
    if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
        Lemur::Scene::SceneManager::Instance().ChangeScene(new GambleScene);


}

void ResultScene::Render(float elapsedTime)
{
}

void ResultScene::DebugImGui()
{
}
