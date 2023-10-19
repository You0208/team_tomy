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

    data quest_data[3];
    data skill_data[3];

    void set_skill_data();
    void set_quest_data();
    //void AllIsSelectReset()
    //{
    //    
    //}

    int skill_num_max = 3;


    struct Card
    {
        WCHAR wcText[256];
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
    int total_point;


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
    float magnification;
    float min_magnification;
    float max_magnification;

    float player_status[3];
    float player_status_max[3];

private:/*---------------- スキル関係 -----------------*/

    Player* player;

    std::shared_ptr<sprite>  spr_back;
    std::shared_ptr<sprite>  spr_card;
    std::shared_ptr<sprite_d>  spr_arrow;
    std::shared_ptr<sprite_d>  spr_select;

    std::shared_ptr<sprite> spr_betbox;
    std::shared_ptr<sprite> spr_coin;
    std::shared_ptr<sprite> spr_OK;
    std::shared_ptr<sprite> spr_betback;
    std::shared_ptr<sprite> spr_small_arrow;
    std::shared_ptr<sprite> spr_number;

};

