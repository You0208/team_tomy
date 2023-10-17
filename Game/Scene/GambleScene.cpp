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

		//player->SetSkill<Tofu>();

		is_first_set_player = true;
	}
	else
	{
		// �������łɐ������Ă���V���O���g���N���X���玝���Ă��邾��
		player = CharacterManager::Instance().GetPlayer();
	}

	// �N�G�X�g�J�[�h���
	wcscpy_s(questCard[0].wcText, L"questA");
	wcscpy_s(questCard[1].wcText, L"questB");
	wcscpy_s(questCard[2].wcText, L"questC");

	// �X�L���J�[�h���
	skillCard[0].position = { 20, 98 };
	skillCard[1].position = { 658, 98 };
	skillCard[2].position = { 1288, 98 };
	skillCard[0].font_position = { 9, 10 };
	skillCard[1].font_position = { 40, 10 };
	skillCard[2].font_position = { 72, 10 };
	wcscpy_s(skillCard[0].wcText, L"skillA");
	wcscpy_s(skillCard[1].wcText, L"skillB");
	wcscpy_s(skillCard[2].wcText, L"skillC");

	// �x�b�g���
	bet_boxsize = { 400,200 };
	bet_boxpos[0] = { 100,800 };
	bet_boxpos[1] = { 600,800 };
	bet_boxpos[2] = { 1100,800 };

	select_decision_pos = { 1600,900 };

	for (int i = 0; i < 3;i++)
	{
		skillCard[i].size = { 600, 780 };
		font_d[i] = 0;
	}

	for (int i = 0; i < 3; ++i)
	{
		small_arrow_up_pos[i] = { 425 + float(i *500),830 };
		small_arrow_down_pos[i] = { 425 + float(i *500),930 };
		char_bet_pos[i] = { 6 + float(i * 25),71 };
		num_bet_pos[i] = { 300 + float(i * 500),1000 };
	}
	// �����ŃV���O���g���N���X�ɃZ�b�g���Ă������Q�[���V�[���œn��
	CharacterManager::Instance().SetPlayer(player);

	// �A�Z�b�g�̃��[�h
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	spr_back = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\gamble_back.png");
	spr_card = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\card.png");
	spr_arrow = std::make_unique<sprite_d>(graphics.GetDevice(), L".\\resources\\Image\\arrow.png");
	spr_select = std::make_unique<sprite_d>(graphics.GetDevice(), L".\\resources\\Image\\select.png");

	spr_betbox = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\bet_space.png");
	spr_coin = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\coin.png");
	spr_OK = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\OK.png");
	spr_betback = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\bet_back.png");
	spr_small_arrow = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\arrow_small.png");
	spr_number = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\number.png");
	// �V�F�[�_�[�̓ǂݍ���
	{
		// sprite�p�f�t�H���g�`��V�F�[�_�[�i�f�B�]���u�j
		{
			D3D11_INPUT_ELEMENT_DESC input_element_desc[]
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			create_vs_from_cso(graphics.GetDevice(),
				"./Shader/sprite_dissolve_vs.cso",
				sprite_vertex_shader.GetAddressOf(),
				sprite_input_layout.GetAddressOf(),
				input_element_desc,
				ARRAYSIZE(input_element_desc));
			create_ps_from_cso(graphics.GetDevice(),
				"./Shader/sprite_dissolve_ps.cso",
				sprite_pixel_shader.GetAddressOf());
		}
	}
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
		// �Z���N�g
		for (int j = 0; j < 2; j++)
		{
			if (mouse.IsArea(select_pos[j].x, select_pos[j].y, 400, 100))
			{
				if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
				{
					// ���肵�����̏���
					step++;
				}
			}
		}
		
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

		switch (selection_card)
		{
		case 0:
			IsDirection = false;
			// �J�[�h���
			questCard[0].position = { 200,98 };
			questCard[1].position = { 740,430 };
			questCard[2].position = { 1280,430 };
			questCard[0].size = { 600,780 };
			questCard[1].size = { 440,565 };
			questCard[2].size = { 440,565 };

			arrow_position[0] = { 820,410 };

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


		step++;
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
		for (int i = 0; i < 3; i++)
		{
			if (mouse.IsArea(small_arrow_up_pos[i].x, small_arrow_up_pos[i].y, 50, 50))
			{
				bet_num[i]++;
			}
			if (mouse.IsArea(small_arrow_down_pos[i].x, small_arrow_down_pos[i].y, 50, 50))
			{
				bet_num[i]--;
			}
		}

		//OK�{�^��
		if (mouse.IsArea(select_decision_pos.x, select_decision_pos.y, 200, 100) )
		{
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				step++;
			}
		}
		break;
	}


    GamePad& game_pad = Input::Instance().GetGamePad();
    if (game_pad.GetButtonDown() & GamePad::BTN_START)
    {
		Lemur::Scene::SceneManager::Instance().ChangeScene(new GameScene);
    }
	if(mouse.GetButtonDown()& Mouse::BTN_RIGHT)
	{
		Lemur::Scene::SceneManager::Instance().ChangeScene(new GameScene);
	}
}

void GambleScene::Render(float elapsedTime)
{
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();

	immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].Get(), 0);
	immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].Get());
	immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].Get(), nullptr, 0xFFFFFFFF);

	immediate_context->IASetInputLayout(sprite_input_layout.Get());
	immediate_context->VSSetShader(sprite_vertex_shader.Get(), nullptr, 0);
	immediate_context->PSSetShader(sprite_pixel_shader.Get(), nullptr, 0);
	immediate_context->PSSetSamplers(0, 1, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());

	SetUpRendering();
	switch (step)
	{
	case Skill_Lottery:

		for (int i = 0; i < 2; i++)spr_select->render(immediate_context, select_pos[i].x, select_pos[i].y, 400, 100);

		for (int i = 0; i < 3; i++)
		{
			// �J�[�h�Q
			spr_card->render(immediate_context, skillCard[i].position.x, skillCard[i].position.y - plusPos[i], skillCard[i].size.x, skillCard[i].size.y);
			// �e�L�X�g
			Lemur::Graphics::Font::Instance().render(skillCard[i].wcText, wcslen(skillCard[i].wcText) + 1, { skillCard[i].font_position.x, skillCard[i].font_position.y-font_d[i] }, 600, 72);
		}

		spr_back->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		break;
	case Quest_Select:

		//spr_card->render(immediate_context, Poo.x, Poo.y - plusPos[0], questCard[0].size.x, questCard[0].size.y);
		spr_card->render(immediate_context, 0, 0 , Poo.x, Poo.y);
		switch (selection_card)
		{
		case 0:
			// ���
			spr_arrow->render(immediate_context, arrow_position[0].x, arrow_position[0].y, arrow_size.x, arrow_size.y);

			// �J�[�h�P
			for (int i = 0; i < 3; i++)
			{
				spr_card->render(immediate_context, questCard[i].position.x, questCard[i].position.y, questCard[i].size.x, questCard[i].size.y);
			}

			spr_back->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

			// �e�L�X�g
			Lemur::Graphics::Font::Instance().render(questCard[0].wcText, wcslen(questCard[0].wcText) + 1, { 16,10 }, 600, 72);
			break;
		case 1:
			// ���
			spr_arrow->render(immediate_context, arrow_position[0].x, arrow_position[0].y, arrow_size.x, arrow_size.y, 1, 1, 1, 1, 180);
			spr_arrow->render(immediate_context, arrow_position[1].x, arrow_position[1].y, arrow_size.x, arrow_size.y);

			// �J�[�h�P
			for (int i = 0; i < 3; i++)
			{
				spr_card->render(immediate_context, questCard[i].position.x, questCard[i].position.y, questCard[i].size.x, questCard[i].size.y);
			}

			spr_back->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			// �e�L�X�g
			Lemur::Graphics::Font::Instance().render(questCard[1].wcText, wcslen(questCard[1].wcText) + 1, { 40,10 }, 600, 72);
			break;
		case 2:
			// ���
			spr_arrow->render(immediate_context, arrow_position[1].x, arrow_position[1].y, arrow_size.x, arrow_size.y, 1, 1, 1, 1, 180);

			// �J�[�h�P
			for (int i = 2; i >= 0; i--)
			{
				spr_card->render(immediate_context, questCard[i].position.x, questCard[i].position.y, questCard[i].size.x, questCard[i].size.y);
			}

			spr_back->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			// �e�L�X�g
			Lemur::Graphics::Font::Instance().render(questCard[2].wcText, wcslen(questCard[2].wcText) + 1, { 63,10}, 600, 72);
			break;
		}

		Lemur::Graphics::Font::Instance().render(L"Quest", 6, { 1,2 }, 600, 72);
		Lemur::Graphics::Font::Instance().render(L"quest select", 13, { 1,8 }, 600, 72);


		break;
	case Gamble_Status:

		//spr_number->render(immediate_context,0, 0, 50, 50,1,1,1,1,0,0,0,50,50);

		for (int i = 0; i < 3; i++)
		{
			//TODO�@���� �����݂�
			spr_number->textout(immediate_context, std::to_string(bet_num[i]), num_bet_pos[i].x, num_bet_pos[i].y,50, 50, 1, 1, 1, 1);
			spr_small_arrow->render(immediate_context, small_arrow_up_pos[i].x, small_arrow_up_pos[i].y, 50, 50);
			spr_small_arrow->render(immediate_context, small_arrow_down_pos[i].x, small_arrow_down_pos[i].y, 50, 50,1,1,1,1,180);
			spr_betbox->render(immediate_context, bet_boxpos[i].x, bet_boxpos[i].y, bet_boxsize.x, bet_boxsize.y);
		}
		spr_OK->render(immediate_context, select_decision_pos.x, select_decision_pos.y, 200, 100);

		spr_betback->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

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



