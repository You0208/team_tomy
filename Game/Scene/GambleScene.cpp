#include "GambleScene.h"

#include <Game/Easing.h>

#include "imgui.h"
#include "Game/Manager/CharacterManager.h"
#include "Game/Skill/AttackSkillDerived.h"
#include "Game/Skill/BadSkillDerived.h"
#include "Game/Skill/SkillDerived.h"
#include "Lemur/Input/Input.h"
#include "Lemur/Scene/SceneManager.h"
#include "SceneGame.h"
#include "Quest.h"
#include "ResultScene.h"
#include "Game/Scene/SceneLoading.h"
#include "Lemur/Audio/AudioManager.h"

//�����̒l��ς����珟��ɓG�̎�ޕς��悤�ɂȂ��Ă܂�
QuestPattern quest_pattern = QuestPattern::C;

// ���݉��E�F�[�u�ڂ�
int wave_count = 1;

// �ŏI�I�ȃ|�C���g�{��
float bet_rate;

void GambleScene::set_skill_data()
{
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();

	wcscpy_s(skill_data[0].title, L"���r");
	wcscpy_s(skill_data[0].contents, L"Skill_ContentsA");
	wcscpy_s(skill_data[1].title, L"�S��");
	wcscpy_s(skill_data[1].contents, L"Skill_ContentsB");
	wcscpy_s(skill_data[2].title, L"����");
	wcscpy_s(skill_data[2].contents, L"Skill_ContentsC");

	wcscpy_s(skill_data[3].title, L"�c��");
	wcscpy_s(skill_data[4].title, L"���Q");
	wcscpy_s(skill_data[5].title, L"����");
	wcscpy_s(skill_data[19].title, L"�Z�p");
	wcscpy_s(skill_data[6].title, L"�z��");
	wcscpy_s(skill_data[7].title, L"����");
	wcscpy_s(skill_data[8].title, L"����");
	wcscpy_s(skill_data[9].title, L"�䖝");
	wcscpy_s(skill_data[10].title, L"�Đ�");
	wcscpy_s(skill_data[18].title, L"�x�e");
	wcscpy_s(skill_data[20].title, L"�t�]");
	wcscpy_s(skill_data[21].title, L"��m");
	wcscpy_s(skill_data[11].title, L"���l");
	wcscpy_s(skill_data[12].title, L"����");
	wcscpy_s(skill_data[13].title, L"����");
	wcscpy_s(skill_data[14].title, L"�얞");
	wcscpy_s(skill_data[15].title, L"����");
	wcscpy_s(skill_data[16].title, L"���f");
	wcscpy_s(skill_data[17].title, L"����");


	spr_skill[0] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/���r.png");
	spr_skill[1] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�S��.png");
	spr_skill[2] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");
	spr_skill[3] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�c��.png");
	spr_skill[4] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");
	spr_skill[5] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�Z�p.png");
	spr_skill[6] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�z��.png");
	spr_skill[7] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");
	spr_skill[8] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");
	spr_skill[9] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�䖝.png");
	spr_skill[10] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�Đ�.png");
	spr_skill[11] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�x�e.png");
	spr_skill[12] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�t�].png");
	spr_skill[13] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/��m.png");
	spr_skill[14] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/card.png");//TODO
	spr_skill[15] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");
	spr_skill[16] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");
	spr_skill[17] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�얞.png");
	spr_skill[18] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");
	spr_skill[19] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/���f.png");
	spr_skill[20] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");
}

void GambleScene::set_quest_data()
{
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();

	wcscpy_s(quest_data[0].title,  L"Quest0");
	wcscpy_s(quest_data[0].contents, L"Quest_ContentsA");
	wcscpy_s(quest_data[1].title,  L"Quest1");
	wcscpy_s(quest_data[1].contents, L"Quest_ContentsB");
	wcscpy_s(quest_data[2].title,  L"Quest2");
	wcscpy_s(quest_data[2].contents, L"Quest_ContentsC");
	wcscpy_s(quest_data[3].title,  L"Quest3");
	wcscpy_s(quest_data[4].title,  L"Quest4");
	wcscpy_s(quest_data[5].title,  L"Quest5");
	wcscpy_s(quest_data[6].title,  L"Quest6");
	wcscpy_s(quest_data[7].title,  L"Quest7");
	wcscpy_s(quest_data[8].title,  L"Quest8");
	wcscpy_s(quest_data[9].title,  L"Quest9");
	wcscpy_s(quest_data[10].title, L"Quest10");
	wcscpy_s(quest_data[11].title, L"Quest11");
	wcscpy_s(quest_data[12].title, L"Quest12");
	wcscpy_s(quest_data[13].title, L"Quest13");
	wcscpy_s(quest_data[14].title, L"Quest14");
	wcscpy_s(quest_data[15].title, L"Quest15");
	wcscpy_s(quest_data[16].title, L"Quest16");

	spr_quest[QuestPattern::A] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/A1.png");
	spr_quest[QuestPattern::B] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/A2.png");
	spr_quest[QuestPattern::C] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/A3.png");
	spr_quest[QuestPattern::D] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/A4.png");
	spr_quest[QuestPattern::E] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/A5.png");
	spr_quest[QuestPattern::F] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/B1.png");
	spr_quest[QuestPattern::G] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/B2.png");
	spr_quest[QuestPattern::H] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/B3.png");
	spr_quest[QuestPattern::I] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/B4.png");
	spr_quest[QuestPattern::J] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/B5.png");
	spr_quest[QuestPattern::K] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/C1.png");
	spr_quest[QuestPattern::L] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/C2.png");
	spr_quest[QuestPattern::M] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/C3.png");
	spr_quest[QuestPattern::N] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/C4.png");
	spr_quest[QuestPattern::O] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/C5.png");//TODO
	spr_quest[QuestPattern::P] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/quest/X.png");

	// �N�G�X�g�p�^�[���̐ݒ�
	for (int quest_num_i = QuestPattern::A; quest_num_i < QuestPattern::MAX_QUEST; quest_num_i++)
		quest_data[quest_num_i].pattern = quest_num_i;

	// �N�G�X�g�̃x�b�g�{�����Z�b�g
	for (int quest_num_i = QuestPattern::A; quest_num_i < QuestPattern::F; quest_num_i++)
	{
		quest_data[quest_num_i].min_magnification = 1.2f;
		quest_data[quest_num_i].max_magnification = 1.5f;
	}
	for (int quest_num_i = QuestPattern::F; quest_num_i < QuestPattern::K; quest_num_i++)
	{
		quest_data[quest_num_i].min_magnification = 1.5f;
		quest_data[quest_num_i].max_magnification = 2.5f;
	}
	for (int quest_num_i = QuestPattern::K; quest_num_i < QuestPattern::P; quest_num_i++)
	{
		quest_data[quest_num_i].min_magnification = 2.0f;
		quest_data[quest_num_i].max_magnification = 3.5f;
	}
	quest_data[QuestPattern::P].min_magnification = 2.0f;
	quest_data[QuestPattern::P].max_magnification = 3.5f;

}

void GambleScene::Initialize()
{
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	SetState();


	// todo ���c����@�A�Z�b�g���[�h���Ĕw�i�Ȃǂ̕`������肢���܂��B
	step = Skill_Lottery;

	// ��T�ڂȂ�v���C���[����
	if (wave_count == 1)
	{
		// �v���C���[�̐���
		player = CreatePlayer();

		// �Q�[���̑S�X�L���̐ݒ�
		player->SetSkill<StrongArm>(L"./resources/Image/���r.png");
		player->SetSkill<DemonPower>(L"./resources/Image/�S��.png");
		player->SetSkill<MagicSword>(L"./resources/Image/����.png");
		player->SetSkill<Cruel>(L"./resources/Image/�c��.png");
		player->SetSkill<Revenge>(L"./resources/Image/���Q.png");
		player->SetSkill<Frenzy>(L"./resources/Image/����.png");
		player->SetSkill<Technique>(L"./resources/Image/�Z�p.png");
		player->SetSkill<BloodSucking>(L"./resources/Image/�z��.png");
		player->SetSkill<Sprint>(L"./resources/Image/����.png");
		player->SetSkill<Acceleration>(L"./resources/Image/����.png");
		player->SetSkill<Patience>(L"./resources/Image/�䖝.png");
		player->SetSkill<Regeneration>(L"./resources/Image/�Đ�.png");
		player->SetSkill<Rest>(L"./resources/Image/�x�e.png");
		player->SetSkill<Reverse>(L"./resources/Image/�t�].png");
		player->SetSkill<Schemer>(L"./resources/Image/��m.png");
		player->SetSkill<SuperMan>(L"./resources/Image/���l.png");
		player->SetSkill<SwordSaint>(L"./resources/Image/����.png");
		player->SetSkill<Gale>(L"./resources/Image/����.png");
		player->SetSkill<Obesity>(L"./resources/Image/�얞.png");

		player->SetSkill<Tofu>(L"./resources/Image/����.png");
		player->SetSkill<Curse>(L"./resources/Image/���f.png");
		player->SetSkill<Arrogance>(L"./resources/Image/����.png");


		// �����ŃV���O���g���N���X�ɃZ�b�g���Ă������Q�[���V�[���œn��
		CharacterManager::Instance().SetPlayer(player);
	}
	else
	{
		// �������łɐ������Ă���V���O���g���N���X���玝���Ă��邾��
		player = CharacterManager::Instance().GetPlayer();
	}
	set_skill_data();
	set_quest_data();


	// �v���C���[�̃X�e�[�^�X���ꎞ�ۑ�
	{
		player_status[0] = player->health;
		player_status[1] = player->attack_power;
		player_status[2] = player->speed_power;
		player_status_max[0] = player->health;
		player_status_max[1] = player->attack_power;
		player_status_max[2] = player->speed_power;
	}
	skill_num_max = player->all_skills.size()-1;
	// �x�b�g���
	bet_boxsize = { 400,200 };

	select_decision_pos = { 1600,900 };

	for (int i = 0; i < 3; ++i)
	{
		// �X�L���J�[�h�ݒ�
		{
			// �ǂ̃X�L���J�[�h��z�z�������������_����
			skillCard[i].category = rand() % skill_num_max;
			// �ʒu
			skillCard[i].position = { 20 + float(i * 630), 98 };
			skillCard[i].font_position = { 10 + float(i * 30), 10 };
			// �T�C�Y
			skillCard[i].size = { 600, 780 };
			// �z�z���ꂽ�ԍ��ɍ��킹����
			//wcscpy_s(skillCard[i].wcText, skill_data[skillCard[i].category].title);
			skillCard[i].wcText = skill_data[skillCard[i].category].title;
		}

		bet_boxpos[i] = { 100 + float(i * 500),800 };
		small_arrow_up_pos[i] = { 425 + float(i * 500),830 };
		small_arrow_down_pos[i] = { 425 + float(i * 500),930 };
		char_bet_pos[i] = { 6 + float(i * 25),71 };
		num_bet_pos[i] = { 300 + float(i * 500),900 };
		coin_bet_pos[i] = { 300 + float(i * 500),700 };
	}
	// �N�G�X�g�J�[�h�ݒ�
	switch (wave_count)
	{
		// �E�F�[�u�P�A�Q
	case 1:
	case 2:
		for (int quest_i = 0; quest_i < 3; quest_i++)
		{
			questCard[quest_i].category = Mathf::RandomRange(quest_data[A].pattern, quest_data[E].pattern);
			//TODO �z����l����i�ЂƂ܂����ԂŁj
		}
			break;
	case 3:
		for (int quest_i = 0; quest_i < 2; quest_i++)
		{
			questCard[quest_i].category = Mathf::RandomRange(static_cast<float>(QuestPattern::F), static_cast<float>(QuestPattern::J));
		}
		questCard[2].category = Mathf::RandomRange(static_cast<float>(QuestPattern::K), static_cast<float>(QuestPattern::O));
		break;
	case 4:
		for (int quest_i = 0; quest_i < 2; quest_i++)
		{
			questCard[quest_i].category = Mathf::RandomRange(static_cast<float>(QuestPattern::F), static_cast<float>(QuestPattern::O));
		}
		questCard[2].category = QuestPattern::P;
		break;
	case 5:
		questCard[1].category = QuestPattern::BOSS;
			break;
	}
	for (int i = 0; i < 3; i++)
	{
		// �z�z���ꂽ�ԍ��ɍ��킹����
		skillCard[i].wcText = skill_data[skillCard[i].category].title;
		questCard[i].wcText = quest_data[questCard[i].category].title;
	}



	/*-------------------------- �A�Z�b�g�̃��[�h -------------------------*/

	// �`���[�g���A���̃y�[�W
	spr_tutorial_01 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/tutorial/skill_page1.png");
	spr_tutorial_02 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/tutorial/skill_page2.png");
	spr_tutorial_03 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/tutorial/skill_page3.png");
	//spr_tutorial_04 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/tutorial/�S.png");
	//spr_tutorial_05 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/tutorial/�T.png");
	//spr_tutorial_06 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/tutorial/�U.png");
	//spr_tutorial_07 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/tutorial/�V.png");
	//spr_tutorial_08 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/tutorial/�W.png");
	//spr_tutorial_09 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/tutorial/�X.png");

	spr_back = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\gamble_back.png");
	spr_card = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\card.png");
	spr_arrow = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\arrow.png");
	spr_select = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\select.png");


	spr_bet_icon[0] = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\bet_heart.png");
	spr_bet_icon[1] = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\bet_attack.png");
	spr_bet_icon[2] = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\bet_speed.png");

	spr_betbox = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\bet_space.png");
	spr_OK = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\OK.png");
	spr_betback = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\bet_back.png");
	spr_small_arrow = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\arrow_small.png");
	spr_number = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\number2.png");

	spr_number = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\number2.png");

	spr_skill_ok = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\Skill_Text_OK.png");
	spr_skill_change = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\Skill_Text_Change.png");


	spr_coin[0] = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\HP_coin.png");
	spr_coin[1] = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\AP_coin.png");
	spr_coin[2] = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\SP_coin.png");

	// todo �����̏��Ԃƃv���C���[�̃Z�b�g�X�L���̏��Ԃ����낦��B
	spr_skill[0] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/���r.png");
	spr_skill[1] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�S��.png");
	spr_skill[2] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");
	spr_skill[3] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�c��.png");
	spr_skill[4] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");
	spr_skill[5] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�Z�p.png");
	spr_skill[6] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�z��.png");
	spr_skill[7] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");
	spr_skill[8] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");
	spr_skill[9] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�䖝.png");
	spr_skill[10] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�Đ�.png");
	spr_skill[11] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�x�e.png");
	spr_skill[12] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�t�].png");
	spr_skill[13] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/��m.png");
	spr_skill[14] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/card.png");//TODO
	spr_skill[15] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");
	spr_skill[16] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");
	spr_skill[17] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/�얞.png");
	spr_skill[18] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");
	spr_skill[19] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/���f.png");
	spr_skill[20] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/����.png");

	spr_skill_back = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/Skill_Back.png");


	Lemur::Audio::AudioManager::Instance().play_bgm(Lemur::Audio::BGM::PLAY, true);
	/*--------------- ����f�o�b�O�p --------------*/
	//Lemur::Scene::SceneManager::Instance().ChangeScene(new GameScene);
}

void GambleScene::Finalize()
{
	// �����ł͂܂��v���C���[���������Ȃ�
	//�Q�[�������S�ɏI������^�C�~���O�ŏ���
}

void GambleScene::Update(HWND hwnd, float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& game_pad = Input::Instance().GetGamePad();

	// ��T�ڂ����`���[�g���A���\��
	if (wave_count == 1)
	{
		switch (easing_step)
		{

		case 0: // 1��ڂ̃C�[�W���O
			if (!EasingTutorial(SCREEN_WIDTH, 0.0f, hide_stop_time_ms, easing_time_ms))
				return;

			// ���ڂ̃C�[�W���O�I��������^�C�}�[�����Z�b�g���ăX�e�b�v��i�߂�B
			ResetEasingTime();
			easing_step++;
			return;

			break;

		case 1: // 2��ڂ̃C�[�W���O
			if (!EasingTutorial(0.0f, -SCREEN_WIDTH, stop_time_ms, easing_time_ms))
				return;


			break;
		}
	}
	switch (step)
	{
	case Skill_Lottery:


		switch (select_down_num)
		{
		case 0:
			if (game_pad.GetButtonDown() & GamePad::BTN_DOWN)
			{
				select_down_num = 1;
			}
			break;
		case 1:
			if (select_down_num > 0)
			{
				if (game_pad.GetButtonDown() & GamePad::BTN_A)
				{
					int n = 0;//�@�Ή����鏇�Ԃ̃X�L��������
					for (const auto& skill : player->all_skills) {
						if (n == skillCard[0].category || n == skillCard[1].category || n == skillCard[2].category)
						{
							lottery_skills.emplace_back(skill.get());
						}
						n++;
					}
					// ���I���ꂽ�X�L���̔z����v���C���[�Ɏ�������B
					player->SetSkill(lottery_skills);
					// �D�揇�ʂŃX�L������ёւ�(Init�Ƃ�update���Ăԏ��Ԃ�ς��邽�߂�)
					player->SkillSort();
					// ���肵�����̏���
					step++;
				}
				if (game_pad.GetButtonDown() & GamePad::BTN_UP)
				{
					select_down_num = 0;
				}
				if (game_pad.GetButtonDown() & GamePad::BTN_RIGHT)
				{
					select_down_num = 2;
				}
			}
			break;
		case 2:
				if (game_pad.GetButtonDown() & GamePad::BTN_A)
				{
					if (!SelectCard[select_num])
					{
						last_num = select_num;
						SelectCard[select_num] = true;
						plusPos[select_num] = 50;
						font_d[select_num] = 4;
					}
					else
					{
						SelectCard[select_num] = false;
						plusPos[select_num] = 0;
						font_d[select_num] = 0;
					}
					for (int i = 0; i < 3; i++)
					{
						// �X�L���J�[�h���Ăєz�蒼��
						if (SelectCard[i])
						{
							skillCard[i].category = rand() % skill_num_max;
							skillCard[i].wcText = skill_data[skillCard[i].category].title;
							// �J�[�h������
							SelectCard[i] = false;
							plusPos[i] = 0;
							font_d[i] = 0;
							// ���I�񐔂����Z
							can_lottery_count--;
						}
					}
				}

				if (select_down_num > 0)
				{
				if (game_pad.GetButtonDown() & GamePad::BTN_UP)
				{
					select_down_num = 0;
				}
				if (game_pad.GetButtonDown() & GamePad::BTN_LEFT)
				{
					select_down_num = 1;
				}
			}
			break;
		}
		if (select_down_num == 0)
		{
			switch (select_num)
			{
			case 0:
				if (game_pad.GetButtonDown() & GamePad::BTN_RIGHT)
				{
					select_num = 1;
				}
				break;
			case 1:
				if (game_pad.GetButtonDown() & GamePad::BTN_RIGHT)
				{
					select_num = 2;
				}
				if (game_pad.GetButtonDown() & GamePad::BTN_LEFT)
				{
					select_num = 0;
				}
				break;
			case 2:
				if (game_pad.GetButtonDown() & GamePad::BTN_LEFT)
				{
					select_num = 1;
				}
				break;
			}

			//// �R���g���[���[
			//if (game_pad.GetButtonDown() & GamePad::BTN_A)
			//{
			//	if (!SelectCard[select_num])
			//	{
			//		last_num = select_num;
			//		SelectCard[select_num] = true;
			//		plusPos[select_num] = 50;
			//		font_d[select_num] = 4;
			//	}
			//	else
			//	{
			//		SelectCard[select_num] = false;
			//		plusPos[select_num] = 0;
			//		font_d[select_num] = 0;
			//	}
			//}
			
			// �J�[�h
			for (int i = 0; i < 3; i++)
			{
				if (SelectCard[i] && last_num != i)
				{
					SelectCard[i] = false;
					plusPos[i] = 0;
					font_d[i] = 0;
				}
				// �}�E�X
				if (mouse.IsArea(skillCard[i].position.x, skillCard[i].position.y - plusPos[i], skillCard[i].size.x, skillCard[i].size.y) && !IsDirection)
				{
					if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
					{
						if (!SelectCard[i])
						{
							last_num = select_num = i;
							SelectCard[i] = true;
							plusPos[i] = 50;
							font_d[i] = 4;
						}
						else
						{
							SelectCard[i] = false;
							plusPos[i] = 0;
							font_d[i] = 0;
						}
					}
				}

			}
		}
		if (can_lottery_count > 0)
		{
			if (mouse.IsArea(select_pos[1].x, select_pos[1].y, 400, 100))
			{
				if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
				{
					for (int i = 0; i < 3; i++)
					{
						// �X�L���J�[�h���Ăєz�蒼��
						if (SelectCard[i])
						{
							skillCard[i].category = rand() % skill_num_max;
							//wcscpy_s(skillCard[i].wcText, skill_data[skillCard[i].category].title);
							skillCard[i].wcText = skill_data[skillCard[i].category].title;
							// �J�[�h������
							SelectCard[i] = false;
							plusPos[i] = 0;
							font_d[i] = 0;
							// ���I�񐔂����Z
							can_lottery_count--;
						}
					}
				}
			}
		}


		if (mouse.IsArea(select_pos[0].x, select_pos[0].y, 400, 100))
		{
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				int n = 0;//�@�Ή����鏇�Ԃ̃X�L��������
				for (const auto& skill : player->all_skills) {
					if (n == skillCard[0].category || n == skillCard[1].category || n == skillCard[2].category)
					{
						lottery_skills.emplace_back(skill.get());
					}
					n++;
				}

				// ���I���ꂽ�X�L���̔z����v���C���[�Ɏ�������B
				player->SetSkill(lottery_skills);
				// �D�揇�ʂŃX�L������ёւ�(Init�Ƃ�update���Ăԏ��Ԃ�ς��邽�߂�)
				player->SkillSort();
				// ���肵�����̏���
				step++;
			}
		}
		break;

	case Quest_Select:

		switch (select_num)
		{
		case 0:
			if (game_pad.GetButtonDown() & GamePad::BTN_RIGHT)
			{
				select_num = 1;
			}
			break;
		case 1:
			if (game_pad.GetButtonDown() & GamePad::BTN_RIGHT)
			{
				select_num = 2;
			}
			if (game_pad.GetButtonDown() & GamePad::BTN_LEFT)
			{
				select_num = 0;
			}
			break;
		case 2:
			if (game_pad.GetButtonDown() & GamePad::BTN_LEFT)
			{
				select_num = 1;
			}
			break;
		}


		switch (selection_card)
		{
		case 0:// �E���I����
			IsDirection = false;
			// �ʒu�̍Đݒ�
			questCard[0].position = { 200,98 };
			questCard[1].position = { 740,430 };
			questCard[2].position = { 1280,430 };
			questCard[0].size = { 600,780 };
			questCard[1].size = { 440,565 };
			questCard[2].size = { 440,565 };

			// ���̈ʒu�ύX
			arrow_position[0] = { 820,410 };

			// �R���g���[���[
			if (game_pad.GetButtonDown() & GamePad::BTN_RIGHT)
			{
				IsDirection = true;
				selection_card = 1;
			}
			// �����N���b�N
			if (mouse.IsArea(arrow_position[0].x, arrow_position[0].y, arrow_size.x, arrow_size.y) && !IsDirection)
			{
				if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
				{
					IsDirection = true;
					selection_card =1;
				}
			}
			break;
		case 1:
			IsDirection = false;
			// �J�[�h���
			questCard[0].position = { 200,430 };
			questCard[1].position = { 658,98 };
			questCard[2].position = { 1280,430 };
			questCard[0].size = { 440,565 };
			questCard[1].size = { 600,780 };
			questCard[2].size = { 440,565 };

			arrow_position[0] = { 548,410 };
			// �R���g���[���[
			if (game_pad.GetButtonDown() & GamePad::BTN_LEFT)
			{
				IsDirection = true;
				selection_card = 0;
			}
			if (mouse.IsArea(arrow_position[0].x, arrow_position[0].y, arrow_size.x, arrow_size.y) && !IsDirection)
			{
				if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
				{
					IsDirection = true;
					selection_card =0;
				}
			}
			// �R���g���[���[
			if (game_pad.GetButtonDown() & GamePad::BTN_RIGHT)
			{
				IsDirection = true;
				selection_card = 2;
			}
			arrow_position[1] = { 1280,410 };
			if (mouse.IsArea(arrow_position[1].x, arrow_position[1].y, arrow_size.x, arrow_size.y) && !IsDirection)
			{
				if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
				{
					IsDirection = true;
					selection_card = 2;
				}
			}
			break;
		case 2:
			IsDirection = false;
			// �J�[�h���
			questCard[0].position = { 200,430 };
			questCard[1].position = { 740,430 };
			questCard[2].position = { 1120,98 };
			questCard[0].size = { 440,565 };
			questCard[1].size = { 440,565 };
			questCard[2].size = { 600,780 };

			arrow_position[1] = { 1000,410 };
			// �R���g���[���[
			if (game_pad.GetButtonDown() & GamePad::BTN_LEFT)
			{
				IsDirection = true;
				selection_card = 1;
			}
			if (mouse.IsArea(arrow_position[1].x, arrow_position[1].y, arrow_size.x, arrow_size.y) && !IsDirection)
			{
				if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
				{
					IsDirection = true;
					selection_card = 1;
				}
			}
			break;
		}
		// �R���g���[���[
		if (game_pad.GetButtonDown() & GamePad::BTN_A)
		{
			quest_pattern = QuestPattern(quest_data[selection_card].pattern);
			bet_rate = quest_data[selection_card].min_magnification;
			min_magnification = quest_data[selection_card].min_magnification;
			max_magnification = quest_data[selection_card].max_magnification;
			step++;
		}
		if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
		{
			quest_pattern = static_cast<QuestPattern>(questCard[selection_card].category);
			bet_rate = quest_data[selection_card].min_magnification;
			min_magnification = quest_data[selection_card].min_magnification;
			max_magnification = quest_data[selection_card].max_magnification;
			step++;
		}


		//step++;
		break;

	case Gamble_Status:

		// todo ���c���� �����ŃM�����u���̃x�b�g�̏���������Ăق����ł��B���肢���܂�
		// �v���C���[��bet_�����Ƀv���C���[�̂�����l�����āA���̓����ʂ��p�����[�^�����������������ė~�����ł��B
		//�Ⴆ�΍U���͂�10�|�C���g�x�b�g����Ȃ牺�̏����݂����ɂȂ�܂��B���͏������Ȃ�����}�W�b�N�i���o�[��attack_power��ݒ肵�Ă邯��
		//�����̏���������Ă��ꂽ�炤�ꂵ���ۂ�B
		//player->bet_AP = 10;
		//player->attack_power = 40;
		//small_arrow_up_pos[0] = Poo;
		// �㉺


		// �R���g���[���[
	{
		switch (select_num)
		{
		case 0:
			if (game_pad.GetButtonDown() & GamePad::BTN_RIGHT)
			{
				select_num = 1;
			}
			break;
		case 1:
			if (game_pad.GetButtonDown() & GamePad::BTN_RIGHT)
			{
				select_num = 2;
			}
			if (game_pad.GetButtonDown() & GamePad::BTN_LEFT)
			{
				select_num = 0;
			}
			break;
		case 2:
			if (game_pad.GetButtonDown() & GamePad::BTN_LEFT)
			{
				select_num = 1;
			}
			break;
		}


		if (bet_rate <= max_magnification && bet_rate >= min_magnification)// �{�����͈͓��̎��ɓ���
		{
			if (game_pad.GetButtonDown() & GamePad::BTN_UP)
			{
				if (player_status[select_num] > 1)
				{
					bet_num[select_num]++;
					player_status_bet[select_num]++;
					player_status[select_num]--;
					player->total_point = bet_num[0] + bet_num[1] + bet_num[2];
					if (player->total_point % 10 == 0 && player->total_point >= 10 && bet_rate <= max_magnification)
					{
						bet_rate += 0.1f;
					}
				}
			}
			if (game_pad.GetButtonDown() & GamePad::BTN_DOWN)
			{
				if (player_status[select_num] < player_status_max[select_num])
				{
					if (bet_num[select_num] > 0)bet_num[select_num]--;
					player_status_bet[select_num]--;
					player_status[select_num]++;
					player->total_point = bet_num[0] + bet_num[1] + bet_num[2];
					if (player->total_point % 10 == 0 && player->total_point >= 10 && bet_rate >= min_magnification)
					{
						bet_rate -= 0.1f;
					}
				}
			}
			if (game_pad.GetButtonDown() & GamePad::BTN_A)
			{
				// �|�C���g��{���������Ă���
				//total_point *= bet_rate;
				// ���̃X�e�[�^�X�����炷

				//player->health = player_status[0];
				//player->max_health = player_status[0] * bet_rate;
				//player->attack_power = player_status[1] * bet_rate;
				//player->speed_power = player_status[2] * bet_rate;

				// ���Ԃ񂱂�
				player->max_health = player_status[0];
				player->attack_power = player_status[1];
				player->speed_power = player_status[2];

				player->bet_MHP = player_status_bet[0];
				player->bet_AP = player_status_bet[1];
				player->bet_SP = player_status_bet[2];
				step++;
			}
		}
	}

		if (bet_rate <= max_magnification && bet_rate >= min_magnification)// �{�����͈͓��̎��ɓ���
		{
			for (int i = 0; i < 3; i++)
			{
				if (mouse.IsArea(small_arrow_up_pos[i].x, small_arrow_up_pos[i].y, 50, 50))
				{
					if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
					{
						if (player_status[i] > 1)
						{
							bet_num[i]++;
							player_status_bet[i]++;
							player_status[i]--;
							player->total_point = bet_num[0] + bet_num[1] + bet_num[2];
							if (player->total_point % 10 == 0 && player->total_point >= 10 && bet_rate <= max_magnification)
							{
								bet_rate += 0.1f;
							}
						}
					}
				}
				if (mouse.IsArea(small_arrow_down_pos[i].x, small_arrow_down_pos[i].y, 50, 50))
				{
					if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
					{
						if (player_status[i] < player_status_max[i])
						{
							if (bet_num[i] > 0)bet_num[i]--;
							player_status_bet[i]--;
							player_status[i]++;
							player->total_point = bet_num[0] + bet_num[1] + bet_num[2];
							if (player->total_point % 10 == 0 && player->total_point >= 10 && bet_rate >= min_magnification)
							{
								bet_rate -= 0.1f;
							}
						}
					}
				}

			}

			player->total_point = bet_num[0] + bet_num[1] + bet_num[2];
		}


		//OK�{�^��
		if (mouse.IsArea(select_decision_pos.x, select_decision_pos.y, 200, 100) )
		{
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				// �|�C���g��{���������Ă���
				//total_point *= bet_rate;
				// ���̃X�e�[�^�X�����炷

				//player->health = player_status[0];
				//player->max_health = player_status[0] * bet_rate;
				//player->attack_power = player_status[1] * bet_rate;
				//player->speed_power = player_status[2] * bet_rate;

			    // ���Ԃ񂱂�
				player->max_health = player_status[0] ;
				player->attack_power = player_status[1];
				player->speed_power = player_status[2];

				player->bet_MHP = player_status_bet[0];
				player->bet_AP = player_status_bet[1];
				player->bet_SP = player_status_bet[2];
				step++;
			}
		}
		break;

	case Loading_Transition:

		// todo �����Ȃ񂩊ȒP�ȉ��o����Ă���������(�]�T�����)
		//player->TestSkillSet("Rest");
		//player->TestSkillSet("Schemer");

		Lemur::Audio::AudioManager::Instance().stop_BGM(Lemur::Audio::BGM::PLAY);
		Lemur::Scene::SceneManager::Instance().ChangeScene(new LoadingScene(new GameScene));
		//Lemur::Scene::SceneManager::Instance().ChangeScene(new ResultScene(true));

		break;
	}
  //  GamePad& game_pad = Input::Instance().GetGamePad();
  //  if (game_pad.GetButtonDown() & GamePad::BTN_START)
  //  {
		////Lemur::Scene::SceneManager::Instance().ChangeScene(new GameScene);
  //  }
	//if(mouse.GetButtonDown()& Mouse::BTN_RIGHT)
	//{
	//	Lemur::Scene::SceneManager::Instance().ChangeScene(new GameScene);
	//}
}

void GambleScene::Render(float elapsedTime)
{
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// �����_�[�^�[�Q�b�g���̐ݒ�ƃN���A
	FLOAT color[]{ 0.2f, 0.2f, 0.2f, 1.0f };
	// �L�����o�X�S�̂��w�肵���F�ɓh��Ԃ�
	immediate_context->ClearRenderTargetView(render_target_view, color);
	// �L�����o�X�S�̂̉��s���������Z�b�g����
	immediate_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// ���ꂩ��`���L�����o�X���w�肷��
	immediate_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	immediate_context->PSSetSamplers(0, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::POINT)].GetAddressOf());
	immediate_context->PSSetSamplers(1, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());
	immediate_context->PSSetSamplers(2, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::ANISOTROPIC)].GetAddressOf());
	immediate_context->PSSetSamplers(3, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_BLACK)].GetAddressOf());
	immediate_context->PSSetSamplers(4, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_WHITE)].GetAddressOf());
	// SHADOW
	immediate_context->PSSetSamplers(5, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::COMPARISON_LINEAR_BORDER_WHITE)].GetAddressOf());


	immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
	immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
	immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);

	immediate_context->IASetInputLayout(sprite_input_layout.Get());
	immediate_context->VSSetShader(sprite_vertex_shader.Get(), nullptr, 0);
	immediate_context->PSSetShader(sprite_pixel_shader.Get(), nullptr, 0);
	immediate_context->PSSetSamplers(0, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());

	switch (step)
	{
	case Skill_Lottery:
		spr_back->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		spr_skill_back->render(immediate_context, skillCard[select_num].position.x - 126, skillCard[select_num].position.y - plusPos[select_num] - 178, 893, 1126);
		for (int i = 0; i < 3; i++)
		{
			spr_skill[skillCard[i].category]->render(immediate_context, skillCard[i].position.x, skillCard[i].position.y - plusPos[i], skillCard[i].size.x, skillCard[i].size.y);
		}

		if(select_down_num>0)		spr_select->render(immediate_context, select_pos[select_down_num-1].x, select_pos[select_down_num-1].y, 400, 100, 1, 1, 1, 1, 0);

		spr_skill_ok->render(immediate_context, select_pos[0].x, select_pos[0].y, 400, 100, 1, 1, 1, 1, 0);
		spr_skill_change->render(immediate_context, select_pos[1].x, select_pos[1].y, 400, 100, 1, 1, 1, 1, 0);
		break;
	case Quest_Select:

		spr_back->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		switch (selection_card)
		{
		case 0:
			// �J�[�h�P
			for (int i = 2; i >= 0; i--)
			{
				spr_quest[questCard[i].category]->render(immediate_context, questCard[i].position.x, questCard[i].position.y, questCard[i].size.x, questCard[i].size.y);
			}
			// ���
			spr_arrow->render(immediate_context, arrow_position[0].x, arrow_position[0].y, arrow_size.x, arrow_size.y, 1, 1, 1, 1, 180);

			break;
		case 1:
			// �J�[�h�P
			for (int i = 0; i < 3; i++)
			{
				spr_quest[questCard[i].category]->render(immediate_context, questCard[i].position.x, questCard[i].position.y, questCard[i].size.x, questCard[i].size.y);
			}
			// ���
			spr_arrow->render(immediate_context, arrow_position[0].x, arrow_position[0].y, arrow_size.x, arrow_size.y);
			spr_arrow->render(immediate_context, arrow_position[1].x, arrow_position[1].y, arrow_size.x, arrow_size.y, 1, 1, 1, 1, 180);

			break;
		case 2:
			// �J�[�h�P
			for (int i = 0; i < 3; i++)
			{
				spr_quest[questCard[i].category]->render(immediate_context, questCard[i].position.x, questCard[i].position.y, questCard[i].size.x, questCard[i].size.y);
			}

			// ���
			spr_arrow->render(immediate_context, arrow_position[1].x, arrow_position[1].y, arrow_size.x, arrow_size.y);
			break;
		}

		Lemur::Graphics::Font::Instance().render(L"Quest", 6, { 1,2 }, 600, 72);
		Lemur::Graphics::Font::Instance().render(L"quest select", 13, { 1,8 }, 600, 72);


		break;
	case Gamble_Status:

		spr_betback->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		{
			std::ostringstream stream;
			stream << std::fixed << std::setprecision(1) << bet_rate;
			std::string result = stream.str();
			spr_number->textout(immediate_context, result, select_decision_pos.x, 50.0f, 50, 50, 1, 1, 1, 1);
		}
		spr_OK->render(immediate_context, select_decision_pos.x, select_decision_pos.y, 200, 100);
		spr_number->textout(immediate_context, std::to_string(player->total_point), select_decision_pos.x, select_decision_pos.y - 70.0f, 50, 50, 1, 1, 1, 1);

		for (int i = 0; i < 3; i++)
		{
			spr_betbox->render(immediate_context, bet_boxpos[i].x, bet_boxpos[i].y, bet_boxsize.x, bet_boxsize.y);
			spr_bet_icon[i]->render(immediate_context, bet_boxpos[i].x, bet_boxpos[i].y,85, 190);
			spr_small_arrow->render(immediate_context, small_arrow_down_pos[i].x, small_arrow_down_pos[i].y, 50, 50, 1, 1, 1, 1, 180);
			spr_small_arrow->render(immediate_context, small_arrow_up_pos[i].x, small_arrow_up_pos[i].y, 50, 50);
			spr_number->textout(immediate_context, std::to_string(bet_num[i]), num_bet_pos[i].x, num_bet_pos[i].y, 50, 50, 1, 1, 1, 1);
			if (bet_num[i] > 0)
			{
				for (int j = 0; j < bet_num[i]; j++)
				{
					spr_coin[i]->render(immediate_context, coin_bet_pos[i].x, coin_bet_pos[i].y - 5 * j, 200, 100);
				}
			}
			spr_number->textout(immediate_context, std::to_string(int(player_status[i])), num_bet_pos[i].x, 100,50, 50, 1, 1, 1, 1);
		}


		Lemur::Graphics::Font::Instance().render(L"HP", 3, { char_bet_pos[0].x,char_bet_pos[0].y }, 600, 72);
		Lemur::Graphics::Font::Instance().render(L"AP", 3, { char_bet_pos[1].x,char_bet_pos[1].y }, 600, 72);
		Lemur::Graphics::Font::Instance().render(L"SP", 3, { char_bet_pos[2].x,char_bet_pos[2].y }, 600, 72);
		break;
	}

	TutorialRender();

	player->DebugImgui();
	DebugImGui();
}

void GambleScene::DebugImGui()
{
	ImGui::Begin("Scene");
	int quest_pattern_int = static_cast<int>(quest_pattern);
	ImGui::InputInt("quest_pattern", &quest_pattern_int);
	quest_pattern = static_cast<QuestPattern>(quest_pattern_int);

	//ImGui::SliderFloat2("Poo", &Poo.x,0,100);
	//ImGui::SliderFloat2("Poo", &Poo.x,0,100);
	//ImGui::SliderInt("last_num", &last_num,0,100);
	ImGui::InputInt("wave_count", &wave_count);
	ImGui::DragFloat2("spr_tutorial_pos", &spr_tutorial_pos.x);
	ImGui::DragFloat("easing_time_ms", &easing_time_ms);
	ImGui::DragFloat("easing_timer_ms", &easing_timer_ms);
	ImGui::DragFloat("stop_time_ms", &stop_time_ms);
	ImGui::DragFloat("stop_timer_ms", &stop_timer_ms);
	ImGui::DragFloat("hide_stop_time_ms", &hide_stop_time_ms);
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

void GambleScene::TutorialRender()
{
	ID3D11DeviceContext* dc = Lemur::Graphics::Graphics::Instance().GetDeviceContext();
	//spr_tutorial_09->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	//spr_tutorial_08->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	//spr_tutorial_07->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	//spr_tutorial_06->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	//spr_tutorial_05->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	//spr_tutorial_04->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	spr_tutorial_03->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	spr_tutorial_02->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	spr_tutorial_01->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
}

bool GambleScene::EasingTutorial(int start_pos,int end_pos,float stop_time_ms,float easing_time_ms)
{
	/*----------- �ŏ��̑ҋ@�̕��� ----------*/

	// �X�g�b�v���鎞�ԗ����ĂȂ�������
	if (stop_timer_ms <= stop_time_ms) {
		// ���Ԑi�߂�
		stop_timer_ms += high_resolution_timer::Instance().time_interval();
		return false;
	}

	/*------------ �C�[�W���O�ŃX���C�h���镔�� -----------*/
	spr_tutorial_pos.x = Easing::OutSine(easing_timer_ms, easing_time_ms, static_cast<float>(end_pos), static_cast<float>(start_pos));

    easing_timer_ms += high_resolution_timer::Instance().time_interval();

	// �C�[�W���O���B������
	if (easing_timer_ms >= easing_time_ms)
	{
		// �C�[�W���O�̃^�C�����~�߂�B
		easing_timer_ms = easing_time_ms;

		// ���̏����I��
		return true;
	}
	else return false;
}



