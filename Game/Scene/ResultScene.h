#pragma once
#include "Game/Player/Player.h"
#include "Lemur/Scene/BaseScene.h"


class ResultScene :
    public Lemur::Scene::BaseScene
{
public:
    // シーン切り替え時に第一引数にクエストをクリアしたかを設定する。
    ResultScene(bool clear_) :clear(clear_){}
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

    enum SceneStep
    {
        Result,// リザルト画面
        Status,// ステータス振り分け画面
    };
    // シーン進行度
    int step = Result;

    Player* player;

    // クリアしたか
    bool clear;

    // for分で描画とか判定とかしたいからplayerの各パラメータを配列化
    float player_status[3];
    // 報酬受け取る前の時点でのプレイヤーパラメータ
    float player_status_min[3];

private:/*--------- スプライト関係 --------*/

    /*------------ スプライトデータ ----------*/
    // クリア時の背景
    // 失敗時の背景

    // 勝利時の背景
    std::shared_ptr<sprite> spr_back_win;
    // 敗北時の背景
    std::shared_ptr<sprite> spr_back_lose;
    // ステータス振り分け時の背景
    std::shared_ptr<sprite> spr_back_status;
    // 矢印
    std::shared_ptr<sprite> spr_small_arrow;
    // 数字
    std::shared_ptr<sprite> spr_number;
    // パラメータの座布団みたいなやつ
    std::shared_ptr<sprite> spr_betbox;
    // どれだけベットしてる可視化する用のコイン
    std::shared_ptr<sprite> spr_coin;
    // OKボタン
    std::shared_ptr<sprite> spr_OK;


    /*------------- スプライト座標 -------------*/

    DirectX::XMFLOAT2 select_decision_pos;

    // 上向き矢印
    DirectX::XMFLOAT2 small_arrow_up_pos[3];
    //下向き矢印
    DirectX::XMFLOAT2 small_arrow_down_pos[3];
    DirectX::XMFLOAT2 char_bet_pos[3];

    DirectX::XMFLOAT2 num_bet_pos[3];

    DirectX::XMFLOAT2 coin_bet_pos[3];

    // パラメータの座布団みたいなやつの位置
    DirectX::XMFLOAT2 bet_boxpos[3];
    // パラメータの座布団みたいなやつのサイズ
    DirectX::XMFLOAT2 bet_boxsize = { 400,200 };


};
