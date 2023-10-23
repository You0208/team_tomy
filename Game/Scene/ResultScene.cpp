#include "Game/Scene/ResultScene.h"

#include "GambleScene.h"
#include "Game/Manager/CharacterManager.h"
#include "Lemur/Input/Input.h"
#include "Lemur/Scene/SceneManager.h"

void ResultScene::Initialize()
{
    // �����ŊG�̐ݒ�
    player = CharacterManager::Instance().GetPlayer();

    // �N���A���Ă����V��^����
    if (clear)
        GiveReward();
}

void ResultScene::Finalize()
{
}

void ResultScene::Update(HWND hwnd, float elapsedTime)
{
    // todo �����̌����ڂ̉��o�ǂ�����H
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



// todo �G�t�F�N�g
// todo ���[�g���[�V����
// todo �M�����u���V�[���̃x�b�g���[�g��ւ�
// todo �X�L��
// todo UI�̂���
// todo �e�N�X�`��
// todo �J�������[�N

// todo ���ƂȂ񂩂��낢��