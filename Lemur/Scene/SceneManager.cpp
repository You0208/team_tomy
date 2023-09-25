#include "SceneManager.h"

namespace Lemur::Scene
{
    // �X�V����
    void SceneManager::Update(HWND hwnd, float elapsedTime)
    {
        if (nextScene != nullptr)
        {
            // �Â��V�[�����I������
            Clear();

            // �V�����V�[����ݒ�
            currentScene = nextScene;
            nextScene = nullptr;

            // �V�[������������
            if (!currentScene->IsReady())
            {
                currentScene->Initialize();
            }
        }

        if (currentScene != nullptr)
        {
            currentScene->Update(hwnd, elapsedTime);
        }
    }

    // �`�揈��
    void SceneManager::Render(float elapsedTime)
    {
        if (currentScene != nullptr)
        {
            currentScene->Render(elapsedTime);
        }
    }

    void SceneManager::Clear()
    {
        if (currentScene != nullptr)
        {
            currentScene->Finalize();
            delete currentScene;
            currentScene = nullptr;
        }
    }

    // �V�[���؂�ւ�
    void SceneManager::ChangeScene(BaseScene* scene)
    {
        nextScene = scene;
    }
}