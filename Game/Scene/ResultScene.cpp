#include "Game/Scene/ResultScene.h"

#include "GambleScene.h"
#include "imgui.h"
#include "Game/Manager/CharacterManager.h"
#include "Lemur/Input/Input.h"
#include "Lemur/Scene/SceneManager.h"

extern float bet_rate;
void ResultScene::Initialize()
{
	step = SceneStep::Result;

    // 絵の設定
    ID3D11Device* device = Lemur::Graphics::Graphics::Instance().GetDevice();
    spr_back_win = std::make_unique<sprite>(device, L".\\resources\\Image\\Result_Win.png");
    spr_back_lose = std::make_unique<sprite>(device, L".\\resources\\Image\\Result_Lose.png");
    spr_back_status = std::make_unique<sprite>(device, L".\\resources\\Image\\pose_back.png");

	spr_betbox = std::make_unique<sprite>(device, L".\\resources\\Image\\bet_space.png");
	spr_OK = std::make_unique<sprite>(device, L".\\resources\\Image\\OK.png");
	spr_small_arrow = std::make_unique<sprite>(device, L".\\resources\\Image\\arrow_small.png");
	spr_number = std::make_unique<sprite>(device, L".\\resources\\Image\\number2.png");

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

    // クリアしてたら報酬を与える
    if (clear)
        GiveReward();
}

void ResultScene::Finalize()
{
}

void ResultScene::Update(HWND hwnd, float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& game_pad = Input::Instance().GetGamePad();
    switch (step)
    {
    case SceneStep::Result: // リザルト画面

		if (game_pad.GetButtonDown() & GamePad::BTN_B||
			mouse.GetButtonDown()&Mouse::BTN_RIGHT)
		{
			if (clear)
				step++;
			else
				Lemur::Scene::SceneManager::Instance().ChangeScene(new GambleScene);
		}

		break;

    case SceneStep::Status: // ステータス振り分け

		// プレイヤーのパラメータを配列に変換(for文で三つのパラメータを一気にやるために)
		player_status[0] = static_cast<float>(player->max_health);
		player_status[1] = player->attack_power;
		player_status[2] = player->speed_power;

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
					player_status[i]++;
				}
			}
			// 下矢印の範囲内でボタンが押されたら
			if (mouse.IsArea(small_arrow_down_pos[i].x, small_arrow_down_pos[i].y, 50, 50) &&
				mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				// 元のパラメータは下回らないようにする
				if (player_status[i] > player_status_min[i])
				{
					player->total_point++;
					player_status[i]--;
				}
			}
		}

		// 変動後のパラメータを実際のプレイヤーにセット
		player->max_health = static_cast<int>(player_status[0]);
		player->attack_power = player_status[1];
		player->speed_power = player_status[2];

		//OKボタン
		if (mouse.IsArea(select_decision_pos.x, select_decision_pos.y, 200, 100) &&
			mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			Lemur::Scene::SceneManager::Instance().ChangeScene(new GambleScene);
		}

		break;
    }
}

void ResultScene::Render(float elapsedTime)
{

    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
    ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
    SetUpRendering();

    switch (step)
    {
    case SceneStep::Result:

		if (clear) spr_back_win->render(graphics.GetDeviceContext(), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
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
			spr_small_arrow->render(immediate_context, small_arrow_down_pos[i].x, small_arrow_down_pos[i].y, 50, 50, 1, 1, 1, 1, 180);
			spr_small_arrow->render(immediate_context, small_arrow_up_pos[i].x, small_arrow_up_pos[i].y, 50, 50);
			//spr_number->textout(immediate_context, std::to_string(bet_num[i]), num_bet_pos[i].x, num_bet_pos[i].y, 25, 50, 1, 1, 1, 1);
			//if (bet_num[i] > 0)
			//{
			//	for (int j = bet_num[i]; j > 0; j--)
			//	{
			//		spr_coin->render(immediate_context, coin_bet_pos[i].x, coin_bet_pos[i].y - 5 * j, 200, 100);
			//	}
			//}
			spr_number->textout(immediate_context, std::to_string(int(player_status[i])), num_bet_pos[i].x, 100, 25, 50, 1, 1, 1, 1);
		}


		Lemur::Graphics::Font::Instance().render(L"HP", 3, { char_bet_pos[0].x,char_bet_pos[0].y }, 600, 72);
		Lemur::Graphics::Font::Instance().render(L"AP", 3, { char_bet_pos[1].x,char_bet_pos[1].y }, 600, 72);
		Lemur::Graphics::Font::Instance().render(L"SP", 3, { char_bet_pos[2].x,char_bet_pos[2].y }, 600, 72);

		break;
    }

    DebugImGui();

}

void ResultScene::DebugImGui()
{
		player->DebugImgui();
	ImGui::Begin("Scene");
	ImGui::InputInt("step", &step);
	ImGui::Checkbox("clear", &clear);
	ImGui::InputFloat("bet_rate", &bet_rate);
	ImGui::End();

}

void ResultScene::GiveReward()
{
    //player->attack_power  += player->bet_AP * bet_rate;
    //player->defense_power += player->bet_DP * bet_rate;
    //player->speed_power   += player->bet_SP * bet_rate;
    //player->max_health    += static_cast<int>(player->bet_MHP * bet_rate);
	player->total_point *= bet_rate;
}



// todo エフェクト
// todo ルートモーション
// todo スキル
// todo カメラワーク

// todo あとなんかいろいろ