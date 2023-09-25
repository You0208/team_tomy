#pragma once

#include "BaseScene.h"

// �V�[���}�l�[�W���[
namespace Lemur::Scene
{
    class SceneManager
    {
    private:
        SceneManager() {}
        ~SceneManager() {}

    public:
        // �B��̃C���X�^���X�擾
        static SceneManager& Instance()
        {
            static SceneManager instance;
            return instance;
        }

        // �X�V����
        void Update(HWND hwnd, float elapsedTime);

        // �`�揈��
        void Render(float elapsedTime);

        // �V�[���N���A
        void Clear();

        // �V�[���̐؂�ւ�
        void ChangeScene(BaseScene* scene);

        void Finalize();
    private:
        BaseScene* currentScene = nullptr;
        BaseScene* nextScene = nullptr;
    };
}