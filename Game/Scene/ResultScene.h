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
    float player_RewordStatus_box[3];
    // 報酬受け取る前の時点でのプレイヤーパラメータ
    float player_status_min[3];

private:/*--------- スプライト関係 --------*/

    /*------------ スプライトデータ ----------*/
    // クリア時の背景
    std::shared_ptr<sprite> spr_back_Clear;
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
    std::shared_ptr<sprite> spr_coin[3];
    // OKボタン
    std::shared_ptr<sprite> spr_OK;

    // チュートリアル
    std::shared_ptr<sprite> tutorial;

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



    /*--------------- チュートリアル --------------*/



    // チュートリアルが終わったか
    bool tutorial_end;
    // チュートリアルページの位置(全員共通の使って、イージングするときに初期化)
    DirectX::XMFLOAT2 spr_tutorial_pos = { 1920.0f,0.0f };


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
    bool EasingTutorial(int start_pos, int end_pos, float stop_time_ms, float easing_time_ms);

    void ResetEasingTime()
    {
        easing_timer_ms = 0.0f;
        stop_timer_ms = 0.0f;
    }
    int easing_step;

};
