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

// todo 牟田さん こいつにQuestPattern型のどのクエストにするのか選択して値を入れる処理を作ってください。お願いします。
//こいつの値を変えたら勝手に敵の種類変わるようになってます
QuestPattern quest_pattern = QuestPattern::B;

// 一番初めのプレイヤーを生成したか(二週目移行か)
bool is_first_set_player = false;
void GambleScene::Initialize()
{
    // todo 牟田さん　アセットロードして背景などの描画をお願いします。
	step = Skill_Lottery;

	if(!is_first_set_player)
	{
		// プレイヤーの生成
	    player = CreatePlayer();

        // ゲームの全スキルの設定
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
		// もうすでに生成してたらシングルトンクラスから持ってくるだけ
		player = CharacterManager::Instance().GetPlayer();
	}

	// クエストカード情報
	wcscpy_s(questCard[0].wcText, L"questA");
	wcscpy_s(questCard[1].wcText, L"questB");
	wcscpy_s(questCard[2].wcText, L"questC");

	// スキルカード情報
	skillCard[0].position = { 20, 98 };
	skillCard[1].position = { 658, 98 };
	skillCard[2].position = { 1288, 98 };
	skillCard[0].size = { 600, 780 };
	skillCard[1].size = { 600, 780 };
	skillCard[2].size = { 600, 780 };

	// ここでシングルトンクラスにセットしてこいつをゲームシーンで渡す
	CharacterManager::Instance().SetPlayer(player);

	// アセットのロード
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	spr_back = std::make_unique<sprite>(graphics.GetDevice(), L".\\resources\\Image\\gamble_back.png");
	spr_card = std::make_unique<sprite_d>(graphics.GetDevice(), L".\\resources\\Image\\card.png");
	spr_arrow = std::make_unique<sprite_d>(graphics.GetDevice(), L".\\resources\\Image\\arrow.png");

	// シェーダーの読み込み
	{
		// sprite用デフォルト描画シェーダー（ディゾルブ）
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
	// ここではまだプレイヤーを消去しない
	//ゲームが完全に終了するタイミングで消す

}

void GambleScene::Update(HWND hwnd, float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();
	// todo 俺　抽選フラグリセットする。
	switch (step)
	{
	case Skill_Lottery:

		for (int i = 0; i < 3; i++)
		{
			if (mouse.IsArea(skillCard[i].position.x, skillCard[i].position.y - plusPos[i], skillCard[i].size.x, skillCard[i].size.y) && !IsDirection)
			{
				if (!SelectCard[i])
				{
					if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
					{
						SelectCard[i] = true;
						plusPos[i] = 50;
					}
				}
				else
				{
					if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
					{
						SelectCard[i] = false;
						plusPos[i] = 0;
					}
				}
			}
		}
		
		// 抽選できる数だけ繰り返す
		if(can_lottery_count>0)
		{
			// todo 牟田さん ここで抽選するかどうかの判定をするからプレイヤーに選ばせれるようにしてください。お願いします。
			// もう一回抽選するときはほしいスキルはキープできるような処理もお願いします。is_selectをfalseにしたらまた抽選されます
			// ImGuiに抽選されてるスキルが表示されるようになってます。


		    // 今はバグるから強制的に三回抽選する
			if (can_lottery_count > 0/*todo 牟田さん ここのif文は(もっかい抽選が選ばれたら)にしてください*/)
			{
				SetLotterySkills();
				can_lottery_count--;
			}
			/*if(// このスキルで決定するを選択したら)
				can_lottery_count = 0;
				*/
		}
		// Todo　ここ最終的に演出が終了したらにしたい
		if (can_lottery_count <= 0)
		{
			// 抽選されたスキルの配列をプレイヤーに持たせる。
			player->SetSkill(lottery_skills);

#if 0 /*----------- ここはテスト用 -----------*/
			player->TestSkillSet("StrongArm");
			player->TestSkillSet("SuperMan");
#endif

			// 優先順位でスキルを並び替え(Initとかupdateを呼ぶ順番を変えるために)
			player->SkillSort();

			//step++;
		}


		break;

	case Quest_Select:

		// todo 牟田さん　ここでクエストの選択です。quest_patternに値を入れてstepをインクリメントしてください。お願いします。

		switch (selection_card)
		{
		case 0:
			IsDirection = false;
			// カード情報
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
			// カード情報
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
			// カード情報
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


		//step++;
		break;

	case Gamble_Status:

		

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

		// カード２
		spr_card->render(immediate_context, 20, 98- plusPos[0], 600, 780);
		// カード２
		spr_card->render(immediate_context, 658, 98- plusPos[1], 600, 780);
		// カード２
		spr_card->render(immediate_context, 1288, 98- plusPos[2], 600, 780);

		spr_back->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		break;
	case Quest_Select:

		switch (selection_card)
		{
		case 0:
			// 矢印
			spr_arrow->render(immediate_context, arrow_position[0].x, arrow_position[0].y, arrow_size.x, arrow_size.y);

			// カード１
			for (int i = 0; i < 3; i++)
			{
				spr_card->render(immediate_context, questCard[i].position.x, questCard[i].position.y, questCard[i].size.x, questCard[i].size.y);
			}

			spr_back->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

			// テキスト
			Lemur::Graphics::Font::Instance().render(questCard[0].wcText, wcslen(questCard[0].wcText) + 1, { 16,10 }, 600, 72);
			break;
		case 1:
			// 矢印
			spr_arrow->render(immediate_context, arrow_position[0].x, arrow_position[0].y, arrow_size.x, arrow_size.y, 1, 1, 1, 1, 180);
			spr_arrow->render(immediate_context, arrow_position[1].x, arrow_position[1].y, arrow_size.x, arrow_size.y);

			// カード１
			for (int i = 0; i < 3; i++)
			{
				spr_card->render(immediate_context, questCard[i].position.x, questCard[i].position.y, questCard[i].size.x, questCard[i].size.y);
			}

			spr_back->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			// テキスト
			Lemur::Graphics::Font::Instance().render(questCard[1].wcText, wcslen(questCard[1].wcText) + 1, { 40,10 }, 600, 72);
			break;
		case 2:
			// 矢印
			spr_arrow->render(immediate_context, arrow_position[1].x, arrow_position[1].y, arrow_size.x, arrow_size.y, 1, 1, 1, 1, 180);

			// カード１
			for (int i = 2; i >= 0; i--)
			{
				spr_card->render(immediate_context, questCard[i].position.x, questCard[i].position.y, questCard[i].size.x, questCard[i].size.y);
			}

			spr_back->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			// テキスト
			Lemur::Graphics::Font::Instance().render(questCard[2].wcText, wcslen(questCard[2].wcText) + 1, { 63,10}, 600, 72);
			break;
		}

		Lemur::Graphics::Font::Instance().render(L"Quest", 6, { 1,2 }, 600, 72);
		Lemur::Graphics::Font::Instance().render(L"quest select", 13, { 1,8 }, 600, 72);

		player->DebugImgui();
		DebugImGui();

		break;
	case Gamble_Status:



		break;
	}

}

void GambleScene::DebugImGui()
{
	ImGui::Begin("Scene");
	ImGui::SliderFloat2("Poo", &Poo.x,0,100);
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

	_ASSERT_EXPR(player->skill_capacity <= all_skill_count, L"取得可能スキル超過");


	//ここのfor文はスキルを三つ抽選するところ
	// スキルを所持できる分だけ繰り返す
	for (int i = 0; i < player->skill_capacity;)
	{
		// ランダム抽選
		BaseSkill* skill = player->all_skills.at(rand() % all_skill_count).get();

		// もうすでに抽選されてたらもう一回
		if (skill->is_select)
		{
			continue;
		}
		skill->is_select = true;

		lottery_skills.emplace_back(skill);
		i++;
	}



}



