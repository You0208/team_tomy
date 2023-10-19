#pragma once
#include "Lemur/Scene/BaseScene.h"
#include <thread>


class LoadingScene :
    public Lemur::Scene::BaseScene
{
public:
    LoadingScene() {}
    ~LoadingScene() override {}

    // ������
    void Initialize()override;

    // �I����
    void Finalize()override;

    // �X�V����
    void Update(HWND hwnd, float elapsedTime)override;

    // �`�揈��
    void Render(float elapsedTime)override;

private:
    // ���[�f�B���O�X���b�h
    static void LoadingThread(LoadingScene* scene);

private:
    BaseScene* nextScene = nullptr;
    std::thread* thread = nullptr;
    std::unique_ptr<sprite> spr_icon;

    DirectX::XMFLOAT2 poo;

    float time;
};

