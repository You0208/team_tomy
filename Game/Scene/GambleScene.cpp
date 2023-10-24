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
// todo 牟田さん こいつにQuestPattern型のどのクエストにするのか選択して値を入れる処理を作ってください。お願いします。
//こいつの値を変えたら勝手に敵の種類変わるようになってます
QuestPattern quest_pattern = QuestPattern::C;

// 現在何ウェーブ目か
int wave_count = 1;

// 最終的なポイント倍率
float bet_rate;

void GambleScene::set_skill_data()
{
	wcscpy_s(skill_data[0].title, L"剛腕");
	wcscpy_s(skill_data[0].contents, L"Skill_ContentsA");
	wcscpy_s(skill_data[1].title, L"鬼力");
	wcscpy_s(skill_data[1].contents, L"Skill_ContentsB");
	wcscpy_s(skill_data[2].title, L"魔剣");
	wcscpy_s(skill_data[2].contents, L"Skill_ContentsC");

	wcscpy_s(skill_data[3].title, L"残酷");
	wcscpy_s(skill_data[4].title, L"復讐");
	wcscpy_s(skill_data[5].title, L"狂乱");
	wcscpy_s(skill_data[19].title, L"技術");
	wcscpy_s(skill_data[6].title, L"吸血");
	wcscpy_s(skill_data[7].title, L"疾走");
	wcscpy_s(skill_data[8].title, L"加速");
	wcscpy_s(skill_data[9].title, L"我慢");
	wcscpy_s(skill_data[10].title, L"再生");
	wcscpy_s(skill_data[18].title, L"休憩");
	wcscpy_s(skill_data[20].title, L"逆転");
	wcscpy_s(skill_data[21].title, L"策士");
	wcscpy_s(skill_data[11].title, L"超人");
	wcscpy_s(skill_data[12].title, L"剣聖");
	wcscpy_s(skill_data[13].title, L"疾風");
	wcscpy_s(skill_data[14].title, L"肥満");
	wcscpy_s(skill_data[15].title, L"豆腐");
	wcscpy_s(skill_data[16].title, L"呪詛");
	wcscpy_s(skill_data[17].title, L"傲慢");
}

void GambleScene::set_quest_data()
{
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



	// クエストパターンの設定
	for (int quest_num_i = QuestPattern::A; quest_num_i < QuestPattern::MAX_QUEST; quest_num_i++)
		quest_data[quest_num_i].pattern = quest_num_i;

	// クエストのベット倍率をセット
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


	// todo 牟田さん　アセットロードして背景などの描画をお願いします。
	step = Skill_Lottery;

	// 一週目ならプレイヤー生成
	if (wave_count == 1)
	{
		// プレイヤーの生成
		player = CreatePlayer();

		// ゲームの全スキルの設定
		player->SetSkill<StrongArm>(L"./resources/Image/剛腕.png");
		player->SetSkill<DemonPower>(L"./resources/Image/鬼力.png");
		player->SetSkill<MagicSword>(L"./resources/Image/魔剣.png");
		player->SetSkill<Cruel>(L"./resources/Image/残酷.png");
		player->SetSkill<Revenge>(L"./resources/Image/復讐.png");
		player->SetSkill<Frenzy>(L"./resources/Image/狂乱.png");
		player->SetSkill<Technique>(L"./resources/Image/技術.png");
		player->SetSkill<BloodSucking>(L"./resources/Image/吸血.png");
		player->SetSkill<Sprint>(L"./resources/Image/疾走.png");
		player->SetSkill<Acceleration>(L"./resources/Image/加速.png");
		player->SetSkill<Patience>(L"./resources/Image/我慢.png");
		player->SetSkill<Regeneration>(L"./resources/Image/再生.png");
		player->SetSkill<Rest>(L"./resources/Image/休憩.png");
		player->SetSkill<Reverse>(L"./resources/Image/逆転.png");
		player->SetSkill<Schemer>(L"./resources/Image/策士.png");
		player->SetSkill<SuperMan>(L"./resources/Image/超人.png");
		player->SetSkill<SwordSaint>(L"./resources/Image/剣聖.png");
		player->SetSkill<Gale>(L"./resources/Image/疾風.png");
		player->SetSkill<Obesity>(L"./resources/Image/肥満.png");

		player->SetSkill<Tofu>(L"./resources/Image/豆腐.png");
		player->SetSkill<Curse>(L"./resources/Image/呪詛.png");
		player->SetSkill<Arrogance>(L"./resources/Image/傲慢.png");


		// ここでシングルトンクラスにセットしてこいつをゲームシーンで渡す
		CharacterManager::Instance().SetPlayer(player);
	}
	else
	{
		// もうすでに生成してたらシングルトンクラスから持ってくるだけ
		player = CharacterManager::Instance().GetPlayer();
	}
	set_skill_data();
	set_quest_data();


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

		bet_boxpos[i] = { 100 + float(i * 500),800 };
		small_arrow_up_pos[i] = { 425 + float(i * 500),830 };
		small_arrow_down_pos[i] = { 425 + float(i * 500),930 };
		char_bet_pos[i] = { 6 + float(i * 25),71 };
		num_bet_pos[i] = { 300 + float(i * 500),900 };
		coin_bet_pos[i] = { 300 + float(i * 500),700 };
	}
	// クエストカード設定
	switch (wave_count)
	{
		// ウェーブ１、２
	case 1:
	case 2:
		for (int quest_i = 0; quest_i < 3; quest_i++)
		{
			questCard[quest_i].category = Mathf::RandomRange(quest_data[A].pattern, quest_data[E].pattern);
			//TODO 配り方考える（ひとまず順番で）
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
		// 配布された番号に合わせた文
		skillCard[i].wcText = skill_data[skillCard[i].category].title;
		questCard[i].wcText = quest_data[questCard[i].category].title;
	}


	/*-------------------------- アセットのロード -------------------------*/

	// チュートリアルのページ
	spr_tutorial_01 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/１.png");
	spr_tutorial_02 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/２.png");
	spr_tutorial_03 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/３.png");
	spr_tutorial_04 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/４.png");
	spr_tutorial_05 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/５.png");
	spr_tutorial_06 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/６.png");
	spr_tutorial_07 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/７.png");
	spr_tutorial_08 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/８.png");
	spr_tutorial_09 = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/９.png");


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

	spr_skill[0] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/剛腕.png");
	spr_skill[1] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/鬼力.png");
	spr_skill[2] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/魔剣.png");
	spr_skill[3] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/残酷.png");
	spr_skill[4] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/狂乱.png");
	spr_skill[5] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/技術.png");
	spr_skill[6] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/吸血.png");
	spr_skill[7] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/疾走.png");
	spr_skill[8] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/加速.png");
	spr_skill[9] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/我慢.png");
	spr_skill[10] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/再生.png");
	spr_skill[11] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/休憩.png");
	spr_skill[12] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/逆転.png");
	spr_skill[13] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/策士.png");
	spr_skill[14] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/card.png");//TODO
	spr_skill[15] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/剣聖.png");
	spr_skill[16] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/疾風.png");
	spr_skill[17] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/肥満.png");
	spr_skill[18] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/豆腐.png");
	spr_skill[19] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/呪詛.png");
	spr_skill[20] = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/skill/傲慢.png");

	spr_skill_back = std::make_unique<sprite>(graphics.GetDevice(), L"./resources/Image/Skill_Back.png");


	/*--------------- これデバッグ用 --------------*/
	//Lemur::Scene::SceneManager::Instance().ChangeScene(new GameScene);
}

void GambleScene::Finalize()
{
	// ここではまだプレイヤーを消去しない
	//ゲームが完全に終了するタイミングで消す
}

void GambleScene::Update(HWND hwnd, float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& game_pad = Input::Instance().GetGamePad();
	// todo 俺　抽選フラグリセットする。
	switch (step)
	{
	case Skill_Lottery:

		//if (EasingTutorial()) return;
		EasingTutorial();

		switch (select_num)
		{
		case 0:
			if (game_pad.GetButtonDown() & GamePad::BTN_RIGHT)
			{
				select_num=1;
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
				select_num=1;
			}
			break;
		}
		// コントローラー
		if (game_pad.GetButtonDown() & GamePad::BTN_B)
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
		}
		// カード
		for (int i = 0; i < 3; i++)
		{
			if (SelectCard[i] && last_num != i)
			{
				SelectCard[i] = false;
				plusPos[i] = 0;
				font_d[i] = 0;
			}
			// マウス
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

			if (game_pad.GetButtonDown() & GamePad::BTN_A)
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

		if (game_pad.GetButtonDown() & GamePad::BTN_Y)
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

			// コントローラー
			if (game_pad.GetButtonDown() & GamePad::BTN_RIGHT)
			{
				IsDirection = true;
				selection_card = 1;
			}
			// 矢印をクリック
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
			// カード情報
			questCard[0].position = { 200,430 };
			questCard[1].position = { 658,98 };
			questCard[2].position = { 1280,430 };
			questCard[0].size = { 440,565 };
			questCard[1].size = { 600,780 };
			questCard[2].size = { 440,565 };

			arrow_position[0] = { 548,410 };
			// コントローラー
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
			// コントローラー
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
			// カード情報
			questCard[0].position = { 200,430 };
			questCard[1].position = { 740,430 };
			questCard[2].position = { 1120,98 };
			questCard[0].size = { 440,565 };
			questCard[1].size = { 440,565 };
			questCard[2].size = { 600,780 };

			arrow_position[1] = { 1000,410 };
			// コントローラー
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
		// コントローラー
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

		// todo 牟田さん ここでギャンブルのベットの処理を作ってほしいです。お願いします
		// プレイヤーのbet_○○にプレイヤーのかける値を入れて、その同じ量をパラメータから引く処理を作って欲しいです。
		//例えば攻撃力に10ポイントベットするなら下の処理みたいになります。今は処理がないからマジックナンバーでattack_powerを設定してるけど
		//そこの処理も作ってくれたらうれしいぽよ。
		//player->bet_AP = 10;
		//player->attack_power = 40;
		//small_arrow_up_pos[0] = Poo;
		// 上下


		// コントローラー
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


		if (bet_rate <= max_magnification && bet_rate >= min_magnification)// 倍率が範囲内の時に動く
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
			if (game_pad.GetButtonDown() & GamePad::BTN_B)
			{
				// ポイントを倍率分かけておく
				//total_point *= bet_rate;
				// 元のステータスを減らす

				//player->health = player_status[0];
				//player->max_health = player_status[0] * bet_rate;
				//player->attack_power = player_status[1] * bet_rate;
				//player->speed_power = player_status[2] * bet_rate;

				// たぶんこう
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

		if (bet_rate <= max_magnification && bet_rate >= min_magnification)// 倍率が範囲内の時に動く
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


		//OKボタン
		if (mouse.IsArea(select_decision_pos.x, select_decision_pos.y, 200, 100) )
		{
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				// ポイントを倍率分かけておく
				//total_point *= bet_rate;
				// 元のステータスを減らす

				//player->health = player_status[0];
				//player->max_health = player_status[0] * bet_rate;
				//player->attack_power = player_status[1] * bet_rate;
				//player->speed_power = player_status[2] * bet_rate;

			    // たぶんこう
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

		// todo ここなんか簡単な演出作ってもいいかも(余裕あれば)
		//player->TestSkillSet("Rest");
		//player->TestSkillSet("Schemer");

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

		spr_skill_back->render(immediate_context, skillCard[select_num].position.x - 126, skillCard[select_num].position.y - plusPos[select_num] - 178, 893, 1126);
		for (int i = 0; i < 3; i++)
		{
			//// カード２
			//spr_card->render(immediate_context, skillCard[i].position.x, skillCard[i].position.y - plusPos[i], skillCard[i].size.x, skillCard[i].size.y);
			//// テキスト
			//Lemur::Graphics::Font::Instance().render(skillCard[i].wcText.c_str(), skillCard[i].wcText.size() + 1, { skillCard[i].font_position.x, skillCard[i].font_position.y - font_d[i] }, 600, 72);
			////Lemur::Graphics::Font::Instance().render(skillCard[i].wcText, wcslen(skillCard[i].wcText) + 1, { skillCard[i].font_position.x, skillCard[i].font_position.y - font_d[i] }, 600, 72);
			spr_skill[skillCard[i].category]->render(immediate_context, skillCard[i].position.x, skillCard[i].position.y - plusPos[i], skillCard[i].size.x, skillCard[i].size.y);
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

			Lemur::Graphics::Font::Instance().render(questCard[0].wcText.c_str(), questCard[0].wcText.size()+1, { 16,10 }, 600, 72);
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
			Lemur::Graphics::Font::Instance().render(questCard[1].wcText.c_str(), questCard[1].wcText.size()+1, { 40,10 }, 600, 72);
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
			Lemur::Graphics::Font::Instance().render(questCard[2].wcText.c_str(), questCard[2].wcText.size()+1, { 63,10 }, 600, 72);
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
	ImGui::SliderFloat2("Poo", &Poo.x,0,100);
	ImGui::SliderInt("last_num", &last_num,0,100);
	ImGui::InputInt("wave_count", &wave_count);
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

void GambleScene::TutorialRender()
{
	ID3D11DeviceContext* dc = Lemur::Graphics::Graphics::Instance().GetDeviceContext();
	spr_tutorial_09->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	spr_tutorial_08->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	spr_tutorial_07->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	spr_tutorial_06->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	spr_tutorial_05->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	spr_tutorial_04->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	spr_tutorial_03->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	spr_tutorial_02->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
	spr_tutorial_01->render(dc, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
}

bool GambleScene::EasingTutorial()
{
	spr_tutorial_pos.x = Easing::OutCubic(easing_timer_ms, easing_time_ms, static_cast<float>(SCREEN_WIDTH),0.0f);
	easing_timer_ms += high_resolution_timer::Instance().time_interval();

	if (easing_timer_ms >= easing_time_ms) return false;
	else return true;
}



