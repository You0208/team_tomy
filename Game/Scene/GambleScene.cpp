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
#include "Game/Scene/SceneLoading.h"
// todo ���c���� ������QuestPattern�^�̂ǂ̃N�G�X�g�ɂ���̂��I�����Ēl�����鏈��������Ă��������B���肢���܂��B
//�����̒l��ς����珟��ɓG�̎�ޕς��悤�ɂȂ��Ă܂�
QuestPattern quest_pattern = QuestPattern::B;

void GambleScene::set_skill_data()
{
	wcscpy_s(skill_data[0].title, L"SkillA");
	wcscpy_s(skill_data[0].contents, L"Skill_ContentsA");
	wcscpy_s(skill_data[1].title, L"SkillB");
	wcscpy_s(skill_data[1].contents, L"Skill_ContentsB");
	wcscpy_s(skill_data[2].title, L"SkillC");
	wcscpy_s(skill_data[2].contents, L"Skill_ContentsC");
}

void GambleScene::set_quest_data()
{
	wcscpy_s(quest_data[0].title, L"QuestA");
	wcscpy_s(quest_data[0].contents, L"Quest_ContentsA");
	wcscpy_s(quest_data[1].title, L"QuestB");
	wcscpy_s(quest_data[1].contents, L"Quest_ContentsB");
	wcscpy_s(quest_data[2].title, L"QuestC");
	wcscpy_s(quest_data[2].contents, L"Quest_ContentsC");

	quest_data[0].min_magnification = 1.2f;
	quest_data[1].min_magnification = 1.5f;
	quest_data[2].min_magnification = 2.0f;

	quest_data[0].max_magnification = 1.5f;
	quest_data[1].max_magnification = 2.5f;
	quest_data[2].max_magnification = 3.5f;


	quest_data[0].pattern = QuestPattern::A;
	quest_data[1].pattern = QuestPattern::B;
	quest_data[2].pattern = QuestPattern::C;
}

// ��ԏ��߂̃v���C���[�𐶐�������(��T�ڈڍs��)
bool is_first_set_player = false;

void GambleScene::Initialize()
{
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	SetState();


    // todo ���c����@�A�Z�b�g���[�h���Ĕw�i�Ȃǂ̕`������肢���܂��B
	step = Skill_Lottery;
	set_skill_data();
	set_quest_data();
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

		//player->SetSkill<Tofu>();

		is_first_set_player = true;
	}
	else
	{
		// �������łɐ������Ă���V���O���g���N���X���玝���Ă��邾��
		player = CharacterManager::Instance().GetPlayer();
	}

	// �v���C���[�̃X�e�[�^�X���ꎞ�ۑ�
	{
		player_status[0] = player->health;
		player_status[1] = player->attack_power;
		player_status[2] = player->speed_power;
		player_status_max[0] = player->health;
		player_status_max[1] = player->attack_power;
		player_status_max[2] = player->speed_power;
	}
	skill_num_max = player->all_skills.size();
	// �x�b�g���
	bet_boxsize = { 400,200 };

	select_decision_pos = { 1600,900 };

	for (int i = 0; i < 3; ++i)
	{
		// �X�L���J�[�h�ݒ�
		{
			// �ǂ̃X�L���J�[�h��z�z�������������_����
			skillCard[i].category = rand() % skill_num_max - 1;
			// �ʒu
			skillCard[i].position = { 20 + float(i * 630), 98 };
			skillCard[i].font_position = { 10 + float(i * 30), 10 };
			// �T�C�Y
			skillCard[i].size = { 600, 780 };
			// �z�z���ꂽ�ԍ��ɍ��킹����
			wcscpy_s(skillCard[i].wcText, skill_data[skillCard[i].category].title);
		}

		// �N�G�X�g�J�[�h�ݒ�
		{
			//TODO �z����l����i�ЂƂ܂����ԂŁj
			questCard[i].category = i;
			// �z�z���ꂽ�ԍ��ɍ��킹����
			wcscpy_s(questCard[i].wcText, quest_data[questCard[i].category].title);
		}

		bet_boxpos[i] = { 100 + float(i * 500),800 };
		small_arrow_up_pos[i] = { 425 + float(i *500),830 };
		small_arrow_down_pos[i] = { 425 + float(i *500),930 };
		char_bet_pos[i] = { 6 + float(i * 25),71 };
		num_bet_pos[i] = { 300 + float(i * 500),900 };
		coin_bet_pos[i] = { 300 + float(i * 500),700 };
	}
	// �����ŃV���O���g���N���X�ɃZ�b�g���Ă������Q�[���V�[���œn��
	CharacterManager::Instance().SetPlayer(player);

	// �A�Z�b�g�̃��[�h
	spr_back = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\gamble_back.png");
	spr_card = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\card.png");
	spr_arrow = std::make_unique<sprite_d>(graphics.GetDevice(), L".\\resources\\Image\\arrow.png");
	spr_select = std::make_unique<sprite_d>(graphics.GetDevice(), L".\\resources\\Image\\select.png");

	spr_betbox = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\bet_space.png");
	spr_coin = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\coin.png");
	spr_OK = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\OK.png");
	spr_betback = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\bet_back.png");
	spr_small_arrow = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\arrow_small.png");
	spr_number = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\number2.png");


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
	// todo ���@���I�t���O���Z�b�g����B
	switch (step)
	{
	case Skill_Lottery:
		// �J�[�h
		for (int i = 0; i < 3; i++)
		{
			if (SelectCard[i] && last_num != i)
			{
				SelectCard[i] = false;
				plusPos[i] = 0;
				font_d[i] = 0;
			}

			if (mouse.IsArea(skillCard[i].position.x, skillCard[i].position.y - plusPos[i], skillCard[i].size.x, skillCard[i].size.y) && !IsDirection)
			{
				if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
				{
					if (!SelectCard[i])
					{
						last_num = i;
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

		if (can_lottery_count > 0)
		{
			// �Z���N�g
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
							wcscpy_s(skillCard[i].wcText, skill_data[skillCard[i].category].title);
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

		// ����
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
		
		
//		// ���I�ł��鐔�����J��Ԃ�
//		if(can_lottery_count>0)
//		{
//			// todo ���c���� �����Œ��I���邩�ǂ����̔�������邩��v���C���[�ɑI�΂����悤�ɂ��Ă��������B���肢���܂��B
//			// ������񒊑I����Ƃ��͂ق����X�L���̓L�[�v�ł���悤�ȏ��������肢���܂��Bis_select��false�ɂ�����܂����I����܂�
//			// ImGui�ɒ��I����Ă�X�L�����\�������悤�ɂȂ��Ă܂��B
//
//
//		    // ���̓o�O�邩�狭���I�ɎO�񒊑I����
//			if (can_lottery_count > 0/*todo ���c���� ������if����(�����������I���I�΂ꂽ��)�ɂ��Ă�������*/)
//			{
//				SetLotterySkills();
//				can_lottery_count--;
//			}
//			/*if(// ���̃X�L���Ō��肷���I��������)
//				can_lottery_count = 0;
//				*/
//		}
//		// Todo�@�����ŏI�I�ɉ��o���I��������ɂ�����
//		if (can_lottery_count <= 0)
//		{
//			// ���I���ꂽ�X�L���̔z����v���C���[�Ɏ�������B
//			player->SetSkill(lottery_skills);
//
//#if 0 /*----------- �����̓e�X�g�p -----------*/
//			player->TestSkillSet("StrongArm");
//			player->TestSkillSet("SuperMan");
//#endif
//
//			// �D�揇�ʂŃX�L������ёւ�(Init�Ƃ�update���Ăԏ��Ԃ�ς��邽�߂�)
//			player->SkillSort();
//
//			//step++;
//		}


		break;

	case Quest_Select:

		// todo ���c����@�����ŃN�G�X�g�̑I���ł��Bquest_pattern�ɒl������step���C���N�������g���Ă��������B���肢���܂��B

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

			// �����N���b�N
			if (mouse.IsArea(arrow_position[0].x, arrow_position[0].y, arrow_size.x, arrow_size.y) && !IsDirection)
			{
				if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
				{
					IsDirection = true;
					selection_card = 1;
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
			if (mouse.IsArea(arrow_position[0].x, arrow_position[0].y, arrow_size.x, arrow_size.y) && !IsDirection)
			{
				if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
				{
					IsDirection = true;
					selection_card = 0;
				}
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

		if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
		{
			quest_pattern = QuestPattern(quest_data[selection_card].pattern);
			magnification = quest_data[selection_card].min_magnification;
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
		player->bet_AP = 10;
		player->attack_power = 40;
		//small_arrow_up_pos[0] = Poo;
		// �㉺

		if (magnification <= max_magnification && magnification >= min_magnification)// �{�����͈͓��̎��ɓ���
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
							player_status[i]--;
							total_point = bet_num[0] + bet_num[1] + bet_num[2];
							if (total_point % 10 == 0 && total_point >= 10 && magnification <= max_magnification)
							{
								magnification += 0.1f;
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
							player_status[i]++;
							total_point = bet_num[0] + bet_num[1] + bet_num[2];
							if (total_point % 10 == 0 && total_point >= 10 && magnification >= min_magnification)
							{
								magnification -= 0.1f;
							}
						}
					}
				}

			}

			total_point = bet_num[0] + bet_num[1] + bet_num[2];
		}


		//OK�{�^��
		if (mouse.IsArea(select_decision_pos.x, select_decision_pos.y, 200, 100) )
		{
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				// �|�C���g��{���������Ă���
				total_point *= magnification;
				// ���̃X�e�[�^�X�����炷
				player->health = player_status[0];
				player->attack_power = player_status[1];
				player->speed_power = player_status[2];
				step++;
			}
		}
		break;

	case Loading_Transition:

		// todo �����Ȃ񂩊ȒP�ȉ��o����Ă���������(�]�T�����)

		Lemur::Scene::SceneManager::Instance().ChangeScene(new LoadingScene(new GameScene));

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


		for (int i = 0; i < 3; i++)
		{
			// �J�[�h�Q
			spr_card->render(immediate_context, skillCard[i].position.x, skillCard[i].position.y - plusPos[i], skillCard[i].size.x, skillCard[i].size.y);
			// �e�L�X�g
			Lemur::Graphics::Font::Instance().render(skillCard[i].wcText, wcslen(skillCard[i].wcText) + 1, { skillCard[i].font_position.x, skillCard[i].font_position.y - font_d[i] }, 600, 72);
		}

		for (int i = 0; i < 2; i++)spr_select->render(immediate_context, select_pos[i].x, select_pos[i].y, 400, 100);

		break;
	case Quest_Select:

		spr_back->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		//spr_card->render(immediate_context, Poo.x, Poo.y - plusPos[0], questCard[0].size.x, questCard[0].size.y);
		//spr_card->render(immediate_context, 0, 0 , Poo.x, Poo.y);
		switch (selection_card)
		{
		case 0:
			// �J�[�h�P
			for (int i = 2; i >= 0; i--)
			{
				spr_card->render(immediate_context, questCard[i].position.x, questCard[i].position.y, questCard[i].size.x, questCard[i].size.y);
			}
			// ���
			spr_arrow->render(immediate_context, arrow_position[0].x, arrow_position[0].y, arrow_size.x, arrow_size.y);

			// �e�L�X�g
			Lemur::Graphics::Font::Instance().render(questCard[0].wcText, wcslen(questCard[0].wcText) + 1, { 16,10 }, 600, 72);
			break;
		case 1:
			// �J�[�h�P
			for (int i = 0; i < 3; i++)
			{
				spr_card->render(immediate_context, questCard[i].position.x, questCard[i].position.y, questCard[i].size.x, questCard[i].size.y);
			}
			// ���
			spr_arrow->render(immediate_context, arrow_position[0].x, arrow_position[0].y, arrow_size.x, arrow_size.y, 1, 1, 1, 1, 180);
			spr_arrow->render(immediate_context, arrow_position[1].x, arrow_position[1].y, arrow_size.x, arrow_size.y);


			// �e�L�X�g
			Lemur::Graphics::Font::Instance().render(questCard[1].wcText, wcslen(questCard[1].wcText) + 1, { 40,10 }, 600, 72);
			break;
		case 2:
			// �J�[�h�P
			for (int i = 0; i < 3; i++)
			{
				spr_card->render(immediate_context, questCard[i].position.x, questCard[i].position.y, questCard[i].size.x, questCard[i].size.y);
			}

			// ���
			spr_arrow->render(immediate_context, arrow_position[1].x, arrow_position[1].y, arrow_size.x, arrow_size.y, 1, 1, 1, 1, 180);

			// �e�L�X�g
			Lemur::Graphics::Font::Instance().render(questCard[2].wcText, wcslen(questCard[2].wcText) + 1, { 63,10 }, 600, 72);
			break;
		}

		Lemur::Graphics::Font::Instance().render(L"Quest", 6, { 1,2 }, 600, 72);
		Lemur::Graphics::Font::Instance().render(L"quest select", 13, { 1,8 }, 600, 72);


		break;
	case Gamble_Status:

		spr_betback->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		{
			std::ostringstream stream;
			stream << std::fixed << std::setprecision(1) << magnification;
			std::string result = stream.str();
			spr_number->textout(immediate_context, result, select_decision_pos.x, 50.0f, 50, 50, 1, 1, 1, 1);
		}
		spr_OK->render(immediate_context, select_decision_pos.x, select_decision_pos.y, 200, 100);
		spr_number->textout(immediate_context, std::to_string(total_point), select_decision_pos.x, select_decision_pos.y - 70.0f, 50, 50, 1, 1, 1, 1);

		for (int i = 0; i < 3; i++)
		{
			spr_betbox->render(immediate_context, bet_boxpos[i].x, bet_boxpos[i].y, bet_boxsize.x, bet_boxsize.y);
			spr_small_arrow->render(immediate_context, small_arrow_down_pos[i].x, small_arrow_down_pos[i].y, 50, 50, 1, 1, 1, 1, 180);
			spr_small_arrow->render(immediate_context, small_arrow_up_pos[i].x, small_arrow_up_pos[i].y, 50, 50);
			spr_number->textout(immediate_context, std::to_string(bet_num[i]), num_bet_pos[i].x, num_bet_pos[i].y, 25, 50, 1, 1, 1, 1);
			if (bet_num[i] > 0)
			{
				for (int j = bet_num[i]; j > 0; j--)
				{
					spr_coin->render(immediate_context, coin_bet_pos[i].x, coin_bet_pos[i].y - 5 * j, 200, 100);
				}
			}
			spr_number->textout(immediate_context, std::to_string(int(player_status[i])), num_bet_pos[i].x, 100,25, 50, 1, 1, 1, 1);
		}


		Lemur::Graphics::Font::Instance().render(L"HP", 3, { char_bet_pos[0].x,char_bet_pos[0].y }, 600, 72);
		Lemur::Graphics::Font::Instance().render(L"AP", 3, { char_bet_pos[1].x,char_bet_pos[1].y }, 600, 72);
		Lemur::Graphics::Font::Instance().render(L"SP", 3, { char_bet_pos[2].x,char_bet_pos[2].y }, 600, 72);
		break;
	}

	player->DebugImgui();
	DebugImGui();
}

void GambleScene::DebugImGui()
{
	ImGui::Begin("Scene");
	ImGui::SliderFloat2("Poo", &Poo.x,0,100);
	ImGui::SliderInt("last_num", &last_num,0,100);
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



