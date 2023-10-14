#include "GambleScene.h"

#include "imgui.h"
#include "Game/Manager/CharacterManager.h"
#include "Game/Skill/AttackSkillDerived.h"
#include "Game/Skill/BadSkillDerived.h"
#include "Game/Skill/SkillDerived.h"
#include "Lemur/Input/Input.h"
#include "Lemur/Scene/SceneManager.h"
#include "SceneGame.h"
#include "Quest.h"

// todo ���c���� ������QuestPattern�^�̂ǂ̃N�G�X�g�ɂ���̂��I�����Ēl�����鏈��������Ă��������B���肢���܂��B
//�����̒l��ς����珟��ɓG�̎�ޕς��悤�ɂȂ��Ă܂�
QuestPattern quest_pattern = QuestPattern::B;

// ��ԏ��߂̃v���C���[�𐶐�������(��T�ڈڍs��)
bool is_first_set_player = false;
void GambleScene::Initialize()
{
    // todo ���c����@�A�Z�b�g���[�h���Ĕw�i�Ȃǂ̕`������肢���܂��B
	step = Skill_Lottery;

	if(!is_first_set_player)
	{
		// �v���C���[�̐���
	    player = CreatePlayer();

        // �Q�[���̑S�X�L���̐ݒ�
		player->SetSkill<StrongArm>();
		player->SetSkill<DemonPower>();
		player->SetSkill<MagicSword>();
		player->SetSkill<Cruel>();
		player->SetSkill<Revenge>();
		player->SetSkill<BloodSucking>();
		player->SetSkill<Sprint>();
		player->SetSkill<Acceleration>();
		player->SetSkill<Patience>();
		player->SetSkill<Regeneration>();
		player->SetSkill<SuperMan>();
		player->SetSkill<SwordSaint>();
		player->SetSkill<Gale>();
		player->SetSkill<Obesity>();

		player->SetSkill<Tofu>();

		is_first_set_player = true;
	}
	else
	{
		// �������łɐ������Ă���V���O���g���N���X���玝���Ă��邾��
		player = CharacterManager::Instance().GetPlayer();
	}


	// �����ŃV���O���g���N���X�ɃZ�b�g���Ă������Q�[���V�[���œn��
	CharacterManager::Instance().SetPlayer(player);


}

void GambleScene::Finalize()
{
	// �����ł͂܂��v���C���[���������Ȃ�
	//�Q�[�������S�ɏI������^�C�~���O�ŏ���

}

void GambleScene::Update(HWND hwnd, float elapsedTime)
{

	switch (step)
	{
	case Skill_Lottery:

		
		// ���I�ł��鐔�����J��Ԃ�
		if(can_lottery_count>0)
		{
			// todo ���c���� �����Œ��I���邩�ǂ����̔�������邩��v���C���[�ɑI�΂����悤�ɂ��Ă��������B���肢���܂��B
			// ������񒊑I����Ƃ��͂ق����X�L���̓L�[�v�ł���悤�ȏ��������肢���܂��Bis_select��false�ɂ�����܂����I����܂�
			// ImGui�ɒ��I����Ă�X�L�����\�������悤�ɂȂ��Ă܂��B

		    // ���̓o�O�邩�狭���I�ɎO�񒊑I����
			if (can_lottery_count > 0/*todo ���c���� ������if����(�����������I���I�΂ꂽ��)�ɂ��Ă�������*/)
			{
				SetLotterySkills();
				can_lottery_count--;
			}
			/*if(// ���̃X�L���Ō��肷���I��������)
				can_lottery_count = 0;
				*/
		}
		// Todo�@�����ŏI�I�ɉ��o���I��������ɂ�����
		if (can_lottery_count <= 0)
		{
			// ���I���ꂽ�X�L���̔z����v���C���[�Ɏ�������B
			player->SetSkill(lottery_skills);

#if 0 /*----------- �����̓e�X�g�p -----------*/
			player->TestSkillSet("StrongArm");
			player->TestSkillSet("SuperMan");
#endif

			// �D�揇�ʂŃX�L������ёւ�(Init�Ƃ�update���Ăԏ��Ԃ�ς��邽�߂�)
			player->SkillSort();

			step++;
		}


		break;

	case Quest_Select:

		// todo ���c����@�����ŃN�G�X�g�̑I���ł��Bquest_pattern�ɒl������step���C���N�������g���Ă��������B���肢���܂��B

		step++;
		break;

	case Gamble_Status:

		

		break;
	}


    GamePad& game_pad = Input::Instance().GetGamePad();
    if (game_pad.GetButtonDown() & GamePad::BTN_START)
    {
		Lemur::Scene::SceneManager::Instance().ChangeScene(new GameScene);
    }
	Mouse& mouse = Input::Instance().GetMouse();
	if(mouse.GetButtonDown()& Mouse::BTN_RIGHT)
	{
		Lemur::Scene::SceneManager::Instance().ChangeScene(new GameScene);

	}
}

void GambleScene::Render(float elapsedTime)
{
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

	SetUpRendering();

	player->DebugImgui();
	DebugImGui();
}

void GambleScene::DebugImGui()
{
	ImGui::Begin("Scene");
	ImGui::Checkbox("is_first_set_player", &is_first_set_player);
	for(auto& skill:lottery_skills)
	{
		ImGui::Text(skill->GetName().c_str());
	}
	ImGui::End();

}

void GambleScene::SetLotterySkills()
{
	int all_skill_count = player->all_skills.size();

	_ASSERT_EXPR(player->skill_capacity <= all_skill_count, L"�擾�\�X�L������");


	//������for���̓X�L�����O���I����Ƃ���
	// �X�L���������ł��镪�����J��Ԃ�
	for (int i = 0; i < player->skill_capacity;)
	{
		// �����_�����I
		BaseSkill* skill = player->all_skills.at(rand() % all_skill_count).get();

		// �������łɒ��I����Ă���������
		if (skill->is_select)
		{
			continue;
		}
		skill->is_select = true;

		lottery_skills.emplace_back(skill);
		i++;
	}



}



