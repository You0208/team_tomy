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


    enum Phase
    {
        Skill,
        Quest,
        Gamble,
    };
    int step;


private:
    Player* CreatePlayer()
    {
        return new Player(
            new PlayerInputComponent(),
            new PlayerPhysicsComponent(),
            new PlayerGraphicsComponent()
        );
    }

private:/*---------------- �X�L���֌W -----------------*/

    // �Q�[���ɑ��݂���S�X�L��
    std::vector<std::unique_ptr<BaseSkill>> all_skills;

    // �X�L����ݒ�
    template<class Skill>
    void SetSkill()
    {
        Skill* skill = new Skill();
        all_skills.emplace_back(skill);
    }

    // �S�X�L�����烉���_���ŃX�L�����擾
    void SetPlayerSkills();

    // ����̃X�L�����e�X�g�������Ƃ��Ɏg��
    void TestSkillSet(const char* set_skill_name)
    {
        BaseSkill* skill = nullptr;
        for (auto& s : all_skills)
        {
            if (s->GetName() == set_skill_name)
                skill = s.get();
        }
        _ASSERT_EXPR(skill, L"�e�X�g�������X�L�����S�X�L���z��ɐݒ肳��Ă܂���");

        player->SetSkill(skill);
    }

    Player* player;
};

