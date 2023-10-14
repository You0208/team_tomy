#pragma once
#include "Game/Player/Player.h"
#include "Game/Skill/BaseSkill.h"
#include "Lemur/Scene/BaseScene.h"

// �X�L���z�z�A�G�N�G�X�g���A�X�e�[�^�X������
//������V�[��
class GambleScene :
    public Lemur::Scene::BaseScene
{
public:
    GambleScene() {}
    ~GambleScene() override {}

    // ������
    void Initialize()override;

    // �I����
    void Finalize()override;

    // �X�V����
    void Update(HWND hwnd, float elapsedTime)override;

    // �`�揈��
    void Render(float elapsedTime)override;

    // �t�F�[�Y
    enum Phase
    {
        Skill_Lottery,// �X�L�����I
        Quest_Select,// �N�G�X�g�I��
        Gamble_Status,// �X�e�[�^�X�M�����u��
    };
    int step;

    void DebugImGui();

private:
    Player* CreatePlayer()
    {
        return new Player(
            new PlayerInputComponent(),
            new PlayerPhysicsComponent(),
            new PlayerGraphicsComponent()
        );
    }

    // ���I�X�L���z��ɃX�L���𒊑I���ăZ�b�g����
    void SetLotterySkills();

    // ���I����Ă�X�L��
    std::vector<BaseSkill*> lottery_skills;

    // ���I�\��
    int can_lottery_count = 3;

private:/*---------------- �X�L���֌W -----------------*/

    Player* player;

};

