#pragma once
#include "Game/Player/Player.h"
#include "Game/Skill/BaseSkill.h"
#include "Lemur/Scene/BaseScene.h"

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

    //void AllIsSelectReset()
    //{
    //    
    //}
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

private:/*---------------- スキル関係 -----------------*/

    Player* player;

};

