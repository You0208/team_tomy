#pragma once
#include "Game/Player/Player.h"
#include "Lemur/Scene/BaseScene.h"


class ResultScene :
    public Lemur::Scene::BaseScene
{
public:
    // シーン切り替え時に第一引数にベットのレート、
    //第二引数にクエストをクリアしたかを設定する。
    ResultScene(float bet_rate_ ,bool clear_) :bet_rate(bet_rate_),clear(clear_){}
    ~ResultScene() override {}

    // 初期化
    void Initialize()override;

    // 終了化
    void Finalize()override;

    // 更新処理
    void Update(HWND hwnd, float elapsedTime)override;

    // 描画処理
    void Render(float elapsedTime)override;

    void DebugImGui();

    // 報酬をプレイヤーに与える
    void GiveReward();
private:
    Player* player;

    float bet_rate;

    // クリアしたか
    bool clear;
};
