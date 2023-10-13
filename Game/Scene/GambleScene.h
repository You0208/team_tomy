#pragma once
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


    enum Phase
    {
        Skill,
        Quest,
        Gamble,
    };
    int step;

private:


};

