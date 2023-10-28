#include "Game/Scene/ResultScene.h"

#include <Game/Easing.h>

#include "GambleScene.h"
#include "imgui.h"
#include "TitleScene.h"
#include "Game/Manager/CharacterManager.h"
#include "Lemur/Input/Input.h"
#include "Lemur/Scene/SceneManager.h"
#include "Lemur/Audio/AudioManager.h"

extern int wave_count;

void ResultScene::Initialize()
{
	step = SceneStep::Result;

    // 絵の設定
    ID3D11Device* device = Lemur::Graphics::Graphics::Instance().GetDevice();
    spr_back_win = std::make_unique<sprite>(device, L".\\resources\\Image\\Result_Win.png");
    spr_back_Clear = std::make_unique<sprite>(device, L".\\resources\\Image\\Result_Clear.png");
    spr_back_lose = std::make_unique<sprite>(device, L".\\resources\\Image\\Result_Over_.png");
    spr_back_status = std::make_unique<sprite>(device, L".\\resources\\Image\\bet_back.png");

	spr_betbox = std::make_unique<sprite>(device, L".\\resources\\Image\\bet_space.png");
	spr_OK = std::make_unique<sprite>(device, L".\\resources\\Image\\OK.png");
	spr_small_arrow = std::make_unique<sprite>(device, L".\\resources\\Image\\arrow_small.png");
	spr_number = std::make_unique<sprite>(device, L".\\resources\\Image\\number2.png");

	spr_coin[0] = std::make_unique<sprite>(device, L".\\resources\\Image\\HP_coin.png");
	spr_coin[1] = std::make_unique<sprite>(device, L".\\resources\\Image\\AP_coin.png");
	spr_coin[2] = std::make_unique<sprite>(device, L".\\resources\\Image\\SP_coin.png");

	spr_bet_icon[0] = std::make_unique<sprite>(device, L".\\resources\\Image\\bet_heart.png");
	spr_bet_icon[1] = std::make_unique<sprite>(device, L".\\resources\\Image\\bet_attack.png");
	spr_bet_icon[2] = std::make_unique<sprite>(device, L".\\resources\\Image\\bet_speed.png");

    tutorial = std::make_unique<sprite>(device, L".\\resources\\Image\\tutorial\\status_page1.png");

	// アセットの座標設定
	for (int i = 0; i < 3; ++i)
	{
		bet_boxpos[i] = { 100 + float(i * 500),800 };
		small_arrow_up_pos[i] = { 425 + float(i * 500),830 };
		small_arrow_down_pos[i] = { 425 + float(i * 500),930 };
		char_bet_pos[i] = { 6 + float(i * 25),71 };
		num_bet_pos[i] = { 300 + float(i * 500),900 };
		coin_bet_pos[i] = { 300 + float(i * 500),700 };
	}
	select_decision_pos = { 1600,900 };

	// プレイヤー取得
    player = CharacterManager::Instance().GetPlayer();

	// ステータス振り分け前の値を保持
	player_status_min[0] = player->health;
	player_status_min[1] = player->attack_power;
	player_status_min[2] = player->speed_power;

	Lemur::Audio::AudioManager::Instance().play_bgm(Lemur::Audio::BGM::OVER, true);
    // クリアしてたら報酬を与える
	if (clear)
	{
		Lemur::Audio::AudioManager::Instance().play_bgm(Lemur::Audio::BGM::CLEAR, true);
		GiveReward();
	}
}

void ResultScene::Finalize()
{
	Lemur::Audio::AudioManager::Instance().stop_AllBGM();
}

void ResultScene::Update(HWND hwnd, float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& game_pad = Input::Instance().GetGamePad();
	float& bet_rate = Lemur::Scene::SceneManager::Instance().bet_rate;

    switch (step)
    {
    case SceneStep::Result: // リザルト画面

		if (game_pad.GetButtonDown() & GamePad::BTN_B||
			mouse.GetButtonDown()&Mouse::BTN_RIGHT)
		{
			if (clear)
			{
				// ボス倒したらクリア
			    if(wave_count==6)
			    {
					Lemur::Scene::SceneManager::Instance().ChangeScene(new TitleScene);
			    }
				step++;
			}
			//負けたら即タイトルに戻る
			else
				Lemur::Scene::SceneManager::Instance().ChangeScene(new TitleScene);
		}

		break;

    case SceneStep::Status: // ステータス振り分け

		// 勝利時に１増えるからここでのwave_countは基本＋１
		if (wave_count == 2)
		{

			switch (easing_step)
			{

			case 0: // 1回目のイージング

				if (!tutorial) break;

				if (!EasingTutorial(SCREEN_WIDTH, 0.0f, hide_stop_time_ms, easing_time_ms))
					return;

				// ボタン押されん限り進めん
				if (game_pad.GetButtonDown() & GamePad::BTN_A ||
					Input::Instance().GetMouse().GetButtonDown() & Mouse::BTN_LEFT)
				{
					// 一回目のイージング終了したらタイマーをリセットしてステップを進める。
					ResetEasingTime();
					stop_time_ms = 0.0f;
					easing_step++;
					return;
				}
				return;

			case 1: // 2回目のイージング
				if (!EasingTutorial(0.0f, -SCREEN_WIDTH, stop_time_ms, easing_time_ms))
					return;
				ResetEasingTime();

				easing_step++;
				stop_time_ms = 3.0f;

				return;

			}
		}


		// todo ここの見た目の演出どうする？
		for (int i = 0; i < 3; i++)
		{
			// 上矢印の範囲内でボタンが押されたら
			if (mouse.IsArea(small_arrow_up_pos[i].x, small_arrow_up_pos[i].y, 50, 50) &&
				mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				// ベットしたトータルポイントが残ってるか
				if (player->total_point > 0)
				{
					// ベットしたトータルポイントを消費してステータス上昇
					player->total_point--;
					player_Reword_Box[i]++;
				}
			}
			// 下矢印の範囲内でボタンが押されたら
			if (mouse.IsArea(small_arrow_down_pos[i].x, small_arrow_down_pos[i].y, 50, 50) &&
				mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				// 元のパラメータは下回らないようにする
				if (player_Reword_Box[i] > player_status_min[i])
				{
					player->total_point++;
					player_Reword_Box[i]--;
				}
			}
		}


		//OKボタン
		if (mouse.IsArea(select_decision_pos.x, select_decision_pos.y, 200, 100) &&
			mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			// 変動後のパラメータを実際のプレイヤーにセット
			player->max_health += static_cast<int>(player_Reword_Box[0]);
			player->attack_power += player_Reword_Box[1];
			player->speed_power += player_Reword_Box[2];

			Lemur::Scene::SceneManager::Instance().ChangeScene(new GambleScene);
		}

		break;
    }
}

void ResultScene::Render(float elapsedTime)
{

    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
	float& bet_rate = Lemur::Scene::SceneManager::Instance().bet_rate;
    SetUpRendering();

    switch (step)
    {
    case SceneStep::Result:

		if (clear) {
			if (wave_count == 6)spr_back_Clear->render(graphics.GetDeviceContext(), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		    else spr_back_win->render(graphics.GetDeviceContext(), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		}
		else      spr_back_lose->render(graphics.GetDeviceContext(), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



        break;

    case SceneStep::Status:
		spr_back_status->render(immediate_context, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
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
			spr_bet_icon[i]->render(immediate_context, bet_boxpos[i].x, bet_boxpos[i].y, 85, 190);
			spr_small_arrow->render(immediate_context, small_arrow_down_pos[i].x, small_arrow_down_pos[i].y, 50, 50, 1, 1, 1, 1, 180);
			spr_small_arrow->render(immediate_context, small_arrow_up_pos[i].x, small_arrow_up_pos[i].y, 50, 50);
			spr_number->textout(immediate_context, std::to_string(player_Reword_Box[i]), num_bet_pos[i].x, num_bet_pos[i].y, 50, 50, 1, 1, 1, 1);
			if (player_Reword_Box[i] > 0)
			{
				for (int j = 0; j < player_Reword_Box[i]; j++)
				{
					spr_coin[i]->render(immediate_context, coin_bet_pos[i].x, coin_bet_pos[i].y - 5 * j, 200, 100);
				}
			}
			spr_number->textout(immediate_context, std::to_string(int(player_Reword_Box[i]+player_status_min[i])), num_bet_pos[i].x, 100, 50, 50, 1, 1, 1, 1);
		}


		Lemur::Graphics::Font::Instance().render(L"HP", 3, { char_bet_pos[0].x,char_bet_pos[0].y }, 600, 72);
		Lemur::Graphics::Font::Instance().render(L"AP", 3, { char_bet_pos[1].x,char_bet_pos[1].y }, 600, 72);
		Lemur::Graphics::Font::Instance().render(L"SP", 3, { char_bet_pos[2].x,char_bet_pos[2].y }, 600, 72);

		if(!tutorial_end)
			tutorial->render(immediate_context, spr_tutorial_pos.x, spr_tutorial_pos.y, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
    }

    DebugImGui();

}

void ResultScene::DebugImGui()
{
	float& bet_rate = Lemur::Scene::SceneManager::Instance().bet_rate;
		player->DebugImgui();
	ImGui::Begin("Scene");
	ImGui::InputInt("step", &step);
	ImGui::Checkbox("clear", &clear);
	ImGui::InputFloat("bet_rate", &bet_rate);
	ImGui::End();

}

void ResultScene::GiveReward()
{
	float& bet_rate = Lemur::Scene::SceneManager::Instance().bet_rate;
    //player->attack_power  += player->bet_AP * bet_rate;
    //player->defense_power += player->bet_DP * bet_rate;
    //player->speed_power   += player->bet_SP * bet_rate;
    //player->max_health    += static_cast<int>(player->bet_MHP * bet_rate);
	player->total_point *= bet_rate;
}

bool ResultScene::EasingTutorial(int start_pos, int end_pos, float stop_time_ms, float easing_time_ms)
{
    /*----------- 最初の待機の部分 ----------*/

	// ストップする時間立ってなかったら
	if (stop_timer_ms <= stop_time_ms) {
		// 時間進める
		stop_timer_ms += high_resolution_timer::Instance().time_interval();
		return false;
	}

	/*------------ イージングでスライドする部分 -----------*/
	spr_tutorial_pos.x = Easing::OutSine(easing_timer_ms, easing_time_ms, static_cast<float>(end_pos), static_cast<float>(start_pos));

	easing_timer_ms += high_resolution_timer::Instance().time_interval();

	// イージング到達したら
	if (easing_timer_ms >= easing_time_ms)
	{
		// イージングのタイムを止める。
		easing_timer_ms = easing_time_ms;

		// 一回の処理終了
		return true;
	}
	else return false;

}


// todo エフェクト
// todo ルートモーション
// todo スキル
// todo カメラワーク

// todo あとなんかいろいろ