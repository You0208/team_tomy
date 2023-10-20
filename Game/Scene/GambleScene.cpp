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
// todo 牟田さん こいつにQuestPattern型のどのクエストにするのか選択して値を入れる処理を作ってください。お願いします。
//こいつの値を変えたら勝手に敵の種類変わるようになってます
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

// 一番初めのプレイヤーを生成したか(二週目移行か)
bool is_first_set_player = false;

void GambleScene::Initialize()
{
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	SetState();


    // todo 牟田さん　アセットロードして背景などの描画をお願いします。
	step = Skill_Lottery;
	set_skill_data();
	set_quest_data();
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

		//player->SetSkill<Tofu>();
		player->SetSkill<Curse>();
		player->SetSkill<Arrogance>();

		is_first_set_player = true;

	    // ここでシングルトンクラスにセットしてこいつをゲームシーンで渡す
	    CharacterManager::Instance().SetPlayer(player);
	}
	else
	{
		// もうすでに生成してたらシングルトンクラスから持ってくるだけ
		player = CharacterManager::Instance().GetPlayer();
	}


	// プレイヤーのステータスを一時保存
	{
		player_status[0] = player->health;
		player_status[1] = player->attack_power;
		player_status[2] = player->speed_power;
		player_status_max[0] = player->health;
		player_status_max[1] = player->attack_power;
		player_status_max[2] = player->speed_power;
	}
	skill_num_max = player->all_skills.size();
	// ベット情報
	bet_boxsize = { 400,200 };

	select_decision_pos = { 1600,900 };

	for (int i = 0; i < 3; ++i)
	{
		// スキルカード設定
		{
			// どのスキルカードを配布したかをランダムで
			skillCard[i].category = rand() % skill_num_max - 1;
			// 位置
			skillCard[i].position = { 20 + float(i * 630), 98 };
			skillCard[i].font_position = { 10 + float(i * 30), 10 };
			// サイズ
			skillCard[i].size = { 600, 780 };
			// 配布された番号に合わせた文
			//wcscpy_s(skillCard[i].wcText, skill_data[skillCard[i].category].title);
			skillCard[i].wcText = skill_data[skillCard[i].category].title;
		}

		// クエストカード設定
		{
			//TODO 配り方考える（ひとまず順番で）
			questCard[i].category = i;
			// 配布された番号に合わせた文
			//wcscpy_s(questCard[i].wcText, quest_data[questCard[i].category].title);
			skillCard[i].wcText = skill_data[skillCard[i].category].title;
		}

		bet_boxpos[i] = { 100 + float(i * 500),800 };
		small_arrow_up_pos[i] = { 425 + float(i *500),830 };
		small_arrow_down_pos[i] = { 425 + float(i *500),930 };
		char_bet_pos[i] = { 6 + float(i * 25),71 };
		num_bet_pos[i] = { 300 + float(i * 500),900 };
		coin_bet_pos[i] = { 300 + float(i * 500),700 };
	}

	// アセットのロード
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


	/*--------------- これデバッグ用 --------------*/
    Lemur::Scene::SceneManager::Instance().ChangeScene(new GameScene);
	player->TestSkillSet("Arrogance");


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
		// カード
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
			// セレクト
			if (mouse.IsArea(select_pos[1].x, select_pos[1].y, 400, 100))
			{
				if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
				{
					for (int i = 0; i < 3; i++)
					{
						// スキルカードを再び配り直し
						if (SelectCard[i])
						{
							skillCard[i].category = rand() % skill_num_max;
							//wcscpy_s(skillCard[i].wcText, skill_data[skillCard[i].category].title);
							skillCard[i].wcText = skill_data[skillCard[i].category].title;
							// カードを元に
							SelectCard[i] = false;
							plusPos[i] = 0;
							font_d[i] = 0;
							// 抽選回数を減算
							can_lottery_count--;
						}
					}
				}
			}
		}

		// 決定
		if (mouse.IsArea(select_pos[0].x, select_pos[0].y, 400, 100))
		{
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				int n = 0;//　対応する順番のスキルを入れる
				for (const auto& skill : player->all_skills) {
					if (n == skillCard[0].category || n == skillCard[1].category || n == skillCard[2].category)
					{
						lottery_skills.emplace_back(skill.get());
					}
					n++;
				}
				// 抽選されたスキルの配列をプレイヤーに持たせる。
				player->SetSkill(lottery_skills);
				// 優先順位でスキルを並び替え(Initとかupdateを呼ぶ順番を変えるために)
				player->SkillSort();
				// 決定した時の処理
				step++;
			}
		}
		
		
//		// 抽選できる数だけ繰り返す
//		if(can_lottery_count>0)
//		{
//			// todo 牟田さん ここで抽選するかどうかの判定をするからプレイヤーに選ばせれるようにしてください。お願いします。
//			// もう一回抽選するときはほしいスキルはキープできるような処理もお願いします。is_selectをfalseにしたらまた抽選されます
//			// ImGuiに抽選されてるスキルが表示されるようになってます。
//
//
//		    // 今はバグるから強制的に三回抽選する
//			if (can_lottery_count > 0/*todo 牟田さん ここのif文は(もっかい抽選が選ばれたら)にしてください*/)
//			{
//				SetLotterySkills();
//				can_lottery_count--;
//			}
//			/*if(// このスキルで決定するを選択したら)
//				can_lottery_count = 0;
//				*/
//		}
//		// Todo　ここ最終的に演出が終了したらにしたい
//		if (can_lottery_count <= 0)
//		{
//			// 抽選されたスキルの配列をプレイヤーに持たせる。
//			player->SetSkill(lottery_skills);
//
//#if 0 /*----------- ここはテスト用 -----------*/
//			player->TestSkillSet("StrongArm");
//			player->TestSkillSet("SuperMan");
//#endif
//
//			// 優先順位でスキルを並び替え(Initとかupdateを呼ぶ順番を変えるために)
//			player->SkillSort();
//
//			//step++;
//		}


		break;

	case Quest_Select:

		// todo 牟田さん　ここでクエストの選択です。quest_patternに値を入れてstepをインクリメントしてください。お願いします。

		switch (selection_card)
		{
		case 0:// 右側選択時
			IsDirection = false;
			// 位置の再設定
			questCard[0].position = { 200,98 };
			questCard[1].position = { 740,430 };
			questCard[2].position = { 1280,430 };
			questCard[0].size = { 600,780 };
			questCard[1].size = { 440,565 };
			questCard[2].size = { 440,565 };

			// 矢印の位置変更
			arrow_position[0] = { 820,410 };

			// 矢印をクリック
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

		// todo 牟田さん ここでギャンブルのベットの処理を作ってほしいです。お願いします
		// プレイヤーのbet_○○にプレイヤーのかける値を入れて、その同じ量をパラメータから引く処理を作って欲しいです。
		//例えば攻撃力に10ポイントベットするなら下の処理みたいになります。今は処理がないからマジックナンバーでattack_powerを設定してるけど
		//そこの処理も作ってくれたらうれしいぽよ。
		player->bet_AP = 10;
		player->attack_power = 40;
		//small_arrow_up_pos[0] = Poo;
		// 上下

		if (magnification <= max_magnification && magnification >= min_magnification)// 倍率が範囲内の時に動く
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


		//OKボタン
		if (mouse.IsArea(select_decision_pos.x, select_decision_pos.y, 200, 100) )
		{
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				// ポイントを倍率分かけておく
				total_point *= magnification;
				// 元のステータスを減らす
				player->health = player_status[0];
				player->attack_power = player_status[1];
				player->speed_power = player_status[2];
				step++;
			}
		}
		break;

	case Loading_Transition:

		// todo ここなんか簡単な演出作ってもいいかも(余裕あれば)

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

	// レンダーターゲット等の設定とクリア
	FLOAT color[]{ 0.2f, 0.2f, 0.2f, 1.0f };
	// キャンバス全体を指定した色に塗りつぶす
	immediate_context->ClearRenderTargetView(render_target_view, color);
	// キャンバス全体の奥行き情報をリセットする
	immediate_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// これから描くキャンバスを指定する
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
			// カード２
			spr_card->render(immediate_context, skillCard[i].position.x, skillCard[i].position.y - plusPos[i], skillCard[i].size.x, skillCard[i].size.y);
			// テキスト
			Lemur::Graphics::Font::Instance().render(skillCard[i].wcText.c_str(), skillCard[i].wcText.size() + 1, { skillCard[i].font_position.x, skillCard[i].font_position.y - font_d[i] }, 600, 72);
			//Lemur::Graphics::Font::Instance().render(skillCard[i].wcText, wcslen(skillCard[i].wcText) + 1, { skillCard[i].font_position.x, skillCard[i].font_position.y - font_d[i] }, 600, 72);
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
			// カード１
			for (int i = 2; i >= 0; i--)
			{
				spr_card->render(immediate_context, questCard[i].position.x, questCard[i].position.y, questCard[i].size.x, questCard[i].size.y);
			}
			// 矢印
			spr_arrow->render(immediate_context, arrow_position[0].x, arrow_position[0].y, arrow_size.x, arrow_size.y);

			// テキスト

			Lemur::Graphics::Font::Instance().render(questCard[0].wcText.c_str(), questCard[0].wcText.size(), { 16,10 }, 600, 72);
		//	Lemur::Graphics::Font::Instance().render(questCard[0].wcText, wcslen(questCard[0].wcText) + 1, { 16,10 }, 600, 72);
			break;
		case 1:
			// カード１
			for (int i = 0; i < 3; i++)
			{
				spr_card->render(immediate_context, questCard[i].position.x, questCard[i].position.y, questCard[i].size.x, questCard[i].size.y);
			}
			// 矢印
			spr_arrow->render(immediate_context, arrow_position[0].x, arrow_position[0].y, arrow_size.x, arrow_size.y, 1, 1, 1, 1, 180);
			spr_arrow->render(immediate_context, arrow_position[1].x, arrow_position[1].y, arrow_size.x, arrow_size.y);


			// テキスト
			Lemur::Graphics::Font::Instance().render(questCard[1].wcText.c_str(), questCard[1].wcText.size(), { 40,10 }, 600, 72);
			break;
		case 2:
			// カード１
			for (int i = 0; i < 3; i++)
			{
				spr_card->render(immediate_context, questCard[i].position.x, questCard[i].position.y, questCard[i].size.x, questCard[i].size.y);
			}

			// 矢印
			spr_arrow->render(immediate_context, arrow_position[1].x, arrow_position[1].y, arrow_size.x, arrow_size.y, 1, 1, 1, 1, 180);

			// テキスト
			Lemur::Graphics::Font::Instance().render(questCard[2].wcText.c_str(), questCard[2].wcText.size(), { 63,10 }, 600, 72);
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



