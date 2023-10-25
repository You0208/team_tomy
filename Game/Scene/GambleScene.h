#pragma once
#include "Game/Player/Player.h"
#include "Game/Skill/BaseSkill.h"
#include "Lemur/Scene/BaseScene.h"
#include "Lemur/Graphics/sprite_d.h"
#include "Lemur/Font/font.h"



// スキル配布、敵クエスト択、ステータスかける
//をするシーン
class GambleScene :
	public Lemur::Scene::BaseScene
{
public:
	GambleScene() {}
	~GambleScene() override {}

	// 初期化
	void Initialize()override;

	// 終了化
	void Finalize()override;

	// 更新処理
	void Update(HWND hwnd, float elapsedTime)override;

	// 描画処理
	void Render(float elapsedTime)override;

	// フェーズ
	enum Phase
	{
		Skill_Lottery,// スキル抽選
		Quest_Select,// クエスト選択
		Gamble_Status,// ステータスギャンブル
		Loading_Transition,// ロードシーンに遷移
	};
	int step;

	void DebugImGui();

	// データ
	struct data
	{
		WCHAR title[256];
		WCHAR contents[256];
		int pattern;
		float min_magnification;
		float max_magnification;
	};

	// クエスト数はボス含めて17個
	data quest_data[30];
	data skill_data[30];

	void set_skill_data();
	void set_quest_data();
	//void AllIsSelectReset()
	//{
	//    
	//}

	int skill_num_max;

	int select_num;

	bool select;

	struct Card
	{
		std::wstring wcText = L"Dummy";
		//WCHAR wcText[256];
		int category;
		DirectX::XMFLOAT2 position;
		DirectX::XMFLOAT2 size;
		DirectX::XMFLOAT2 font_position;
	};
	Card questCard[3];
	Card skillCard[3];
	DirectX::XMFLOAT2 Poo = {};

	DirectX::XMFLOAT2 bet_boxpos[3];
	DirectX::XMFLOAT2 bet_boxsize = { 400,100 };


	float font_d[3];

	int selection_card = 1;
	DirectX::XMFLOAT2 arrow_position[2]{};
	DirectX::XMFLOAT2 arrow_size = { 95, 145 };

	DirectX::XMFLOAT2 select_pos[2] = {
		{448,915},
		{1065,915}
	};

	DirectX::XMFLOAT2 select_decision_pos;

	DirectX::XMFLOAT2 small_arrow_up_pos[3];
	DirectX::XMFLOAT2 small_arrow_down_pos[3];
	DirectX::XMFLOAT2 char_bet_pos[3];

	DirectX::XMFLOAT2 num_bet_pos[3];

	DirectX::XMFLOAT2 coin_bet_pos[3];

	int bet_num[3];



private:
	Player* CreatePlayer()
	{
		return new Player(
			new PlayerInputComponent(),
			new PlayerPhysicsComponent(),
			new PlayerGraphicsComponent()
		);
	}

	// 抽選スキル配列にスキルを抽選してセットする
	void SetLotterySkills();

	// 抽選されてるスキル
	std::vector<BaseSkill*> lottery_skills;

	// 抽選可能回数
	int can_lottery_count = 3;

	bool IsDirection;
	bool SelectCard[3] = {};
	float plusPos[3] = {};
	int last_num;

	// ポイント倍率
	float min_magnification;
	float max_magnification;

	float player_status[3];
	float player_status_bet[3];// プレイヤー側かけたステータスポイント
	float player_status_max[3];

	Player* player;
private:/*---------------- スプライト関係 -----------------*/

	// チュートリアルのページ

	std::unique_ptr<sprite> spr_tutorial_01;
	std::unique_ptr<sprite> spr_tutorial_02;
	std::unique_ptr<sprite> spr_tutorial_03;
	std::unique_ptr<sprite> spr_tutorial_04;
	std::unique_ptr<sprite> spr_tutorial_05;
	std::unique_ptr<sprite> spr_tutorial_06;
	std::unique_ptr<sprite> spr_tutorial_07;
	std::unique_ptr<sprite> spr_tutorial_08;
	std::unique_ptr<sprite> spr_tutorial_09;

	// チュートリアルページの位置(全員共通の使って、イージングするときに初期化)
	DirectX::XMFLOAT2 spr_tutorial_pos = { 1920.0f,0.0f };

	void TutorialRender();

	std::shared_ptr<sprite>  spr_back;
	std::shared_ptr<sprite>  spr_card;
	std::shared_ptr<sprite>  spr_arrow;
	std::shared_ptr<sprite>  spr_select;

	std::shared_ptr<sprite> spr_betbox;
	std::shared_ptr<sprite> spr_coin[3];
	std::shared_ptr<sprite> spr_OK;
	std::shared_ptr<sprite> spr_betback;
	std::shared_ptr<sprite> spr_small_arrow;
	std::shared_ptr<sprite> spr_number;

	std::shared_ptr<sprite> spr_skill_back;

	std::shared_ptr<sprite> spr_skill[22];
	std::shared_ptr<sprite> spr_quest[22];

	std::shared_ptr<sprite> spr_skill_ok;
	std::shared_ptr<sprite> spr_skill_change;

	/*------- イージング関係 --------*/

	// イージングする時間
	float easing_time_ms = 1.0f;
	// イージングしてる時間
	float easing_timer_ms = 0.0f;

	// チュートリアル画面が真ん中でとどまる時間
	float stop_time_ms = 3.0f;
	// チュートリアル画面が右でとどまる時間
	float hide_stop_time_ms = 1.0f;
	// チュートリアル画面がとどまる処理の値が動くタイマー
	float stop_timer_ms = 0.0f;

	// チュートリアル画面がイージングする処理
	//戻り値にはイージングが終了したかが返る
	//第一引数、イージング始める座標
	//第二引数、イージングで止まる座標
	//第三引数、イージング始まるまでの時間
	//第四引数、イージングにかける時間
	bool EasingTutorial(int start_pos, int end_pos,float stop_time_ms, float easing_time_ms);

	void ResetEasingTime()
	{
		easing_timer_ms = 0.0f;
		stop_timer_ms = 0.0f;
	}
	int easing_step;
};

