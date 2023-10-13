#include "GambleScene.h"

#include "Game/Manager/CharacterManager.h"
#include "Game/Skill/AttackSkillDerived.h"
#include "Game/Skill/BadSkillDerived.h"
#include "Game/Skill/SkillDerived.h"
#include "Lemur/Input/Input.h"
#include "Lemur/Scene/SceneManager.h"
#include "SceneGame.h"

void GambleScene::Initialize()
{
    // �A�Z�b�g���[�h



    // �V�[���ɃX�L���̐ݒ�
	SetSkill<StrongArm>();
	SetSkill<DemonPower>();
	SetSkill<MagicSword>();
	SetSkill<Cruel>();
	SetSkill<Revenge>();
	SetSkill<BloodSucking>();
	SetSkill<Sprint>();
	SetSkill<Acceleration>();
	SetSkill<Patience>();
	SetSkill<Regeneration>();
	SetSkill<SuperMan>();
	SetSkill<SwordSaint>();
	SetSkill<Gale>();
	SetSkill<Obesity>();

	SetSkill<Tofu>();

	// �v���C���[�̐���
	player = CreatePlayer();

	// �v���C���[�ɃX�L�����擾������
	SetPlayerSkills();

	// �D�揇�ʂŃX�L������ёւ�(Init�Ƃ�update���Ăԏ��Ԃ�ς��邽�߂�)
	player->SkillSort();

	// �v���C���[��������
	player->Initialize();

	// �v���C���[���L�����N�^�[�}�l�[�W���ɃZ�b�g
	CharacterManager::Instance().SetPlayer(player);

}

void GambleScene::Finalize()
{
	// �����ł͂܂��v���C���[���������Ȃ�
	//�Q�[�������S�ɏI������^�C�~���O�ŏ���

}

void GambleScene::Update(HWND hwnd, float elapsedTime)
{
    GamePad& game_pad = Input::Instance().GetGamePad();
    if (game_pad.GetButtonDown() & GamePad::BTN_START)
    {
		Lemur::Scene::SceneManager::Instance().ChangeScene(new GameScene);
    }
	Mouse& mouse = Input::Instance().GetMouse();
	if(mouse.GetButtonDown()& Mouse::BTN_LEFT)
	{
		Lemur::Scene::SceneManager::Instance().ChangeScene(new GameScene);

	}
}

void GambleScene::Render(float elapsedTime)
{
}

void GambleScene::SetPlayerSkills()
{
	int all_skill_count = all_skills.size();

	_ASSERT_EXPR(player->skill_capacity <= all_skill_count, L"�擾�\�X�L������");

	// �����ł��镪�����J��Ԃ�
	for (int i = 0; i < player->skill_capacity;)
	{
		BaseSkill* skill = all_skills.at(rand() % all_skill_count).get();

		// �������łɎ擾���Ă���������
		if (skill->GetOwner())
		{
			continue;
		}
		player->SetSkill(skill);
		i++;
	}
}


