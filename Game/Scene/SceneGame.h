#pragma once
#include <xaudio2.h>

#include "Game/Enemy/BossSpider.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Stage/Stage.h"
#include "Lemur/Audio/audio.h"
#include "Lemur/Component/DemoEnemy.h"
#include "Lemur/Component/DemoPlayer.h"
#include "Lemur/Scene/BaseScene.h"
class GameScene :public Lemur::Scene::BaseScene
{
public:
    float x;
    float xp;
    float rate;
    GameScene() {}
    ~GameScene() override {}

    // 初期化
    void Initialize()override;

    // 終了化
    void Finalize()override;

    // 更新処理
    void Update(HWND hwnd, float elapsedTime)override;

    // 描画処理
    void Render(float elapsedTime)override;

    void DebugImGui();

    Player* CreatePlayer()
    {
        return new Player(
            new PlayerInputComponent(),
            new PlayerPhysicsComponent(),
            new PlayerGraphicsComponent()
        );
    }

    template<class Spider_T>
    Spider_T* CreateEnemy()
    {
        return new Spider_T(
            new EnemyInputComponent(),
            new EnemyPhysicsComponent(),
            new EnemyGraphicsComponent()
        );
    }
    //BossSpider* CreateBossSpider()
    //{
    //    return new BossSpider(
    //        new EnemyInputComponent(),
    //        new EnemyPhysicsComponent(),
    //        new EnemyGraphicsComponent()
    //    );
    //}


private:// ゲーム関連

    // 敵を生み出す(関数名は仮)
    //選択されたクエストによって生み出す敵が変わる
    void CreateEnemy_KARI();

    // 敵を全員倒したらシーン切り替え
    void QuestClear();

    void QuestFailed();

    // 更新処理止める用
    bool is_pause = false;

    // 勝利時にかけたステータスの増える倍率
    float bet_rate = 1.0f;
    // 勝利時にかけたステータスの増える倍率の最高値
    float bet_rate_max = 1.0f;

    //ポーズ
    void Pause();

    // UI関係の描画
    void UIRender();
    void PauseRender();
    float timer;


    // シェーダー
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> spider_color;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> spider_normal;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> spider_roughness;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> spider_metalness;

    Microsoft::WRL::ComPtr<ID3D11PixelShader> spider_ps;

    //DemoPlayer
    Player* player = nullptr;
    // ボス敵
    Enemy* boss_enemy = nullptr;

    std::unique_ptr<Stage> stage;

    // Audio
    Microsoft::WRL::ComPtr<IXAudio2> xaudio2;
    IXAudio2MasteringVoice* master_voice = nullptr;
    std::unique_ptr<Lemur::Audio::audio> bgm[8];
    std::unique_ptr<Lemur::Audio::audio> se[8];

    // skkind
    std::shared_ptr<skinned_mesh> skinned_meshes[8];

    DirectX::XMFLOAT3 cobe_position{ 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT4 camera_position{ 0.0f, 0.0f, -10.0f, 1.0f };
    DirectX::XMFLOAT4 camera_focus{ 0.0f, 0.0f, 0.0f, 1.0f };

    DirectX::XMFLOAT4 light_direction{ 1.0f, -1.0f,0.9f, 0.0f };

    DirectX::XMFLOAT3 translation{ 0, 0, 0 };
    DirectX::XMFLOAT3 scaling{ 1, 1, 1 };
    DirectX::XMFLOAT3 rotation{ 0, 0, 0 };
    DirectX::XMFLOAT4 material_color{ 1, 1, 1, 1 };

    /*----------------- UI関連 ------------------*/
    // プレイヤーのHPゲージ
    std::unique_ptr<sprite> player_hp_gauge;
    // プレイヤーのHPゲージ座布団
    std::unique_ptr<sprite> player_hp_gauge_zabuton;

    // 敵のHPゲージ
    std::unique_ptr<sprite> enemy_hp_gauge;
    // 敵のHPゲージ座布団
    std::unique_ptr<sprite> enemy_hp_gauge_zabuton;
    // 敵のHPゲージの位置
    DirectX::XMFLOAT3 ene_HP_gauge_pos{640.0f,0.0f,0.0f};

    // ポーズ画面の背景
    std::unique_ptr<sprite>pause_back;
    //ポーズ画面の戦闘に戻る
    std::unique_ptr<sprite>pause_return;
    // ポーズ画面のタイトルに戻る
    std::unique_ptr<sprite>pause_title;
    // ポーズ画面何選択してるか
    std::unique_ptr<sprite>pause_select;
    // ポーズ画面で上選択してるか
    bool is_pause_Up_selected = true;

    // 戦闘に戻るUIの座標
    DirectX::XMFLOAT2 pause_return_UI_pos{50, 190};
    // タイトルに戻るUIの座標
    DirectX::XMFLOAT2 pause_title_UI_pos{50, 350};
    // ポーズ画面の何選択してるかの位置
    DirectX::XMFLOAT2 pause_select_UI_pos[2]{ {84,221},{84,380} };
    // ポーズ画面のUIのサイズ
    DirectX::XMFLOAT2 pause_UI_size{750,100};

    // 操作方法
    std::unique_ptr<sprite>Method_A_Button;
    std::unique_ptr<sprite>Method_B_Button;
    std::unique_ptr<sprite>Method_Y_Button;
    std::unique_ptr<sprite>Method_LBRB_Button;


    // UIデバッグ用
    DirectX::XMFLOAT2 UI_pos[4]
    {
        {1685,740},{1480,820},{1480,600},{1685,570}
    };
private:// シェーダー関連
    std::unique_ptr<framebuffer> framebuffers[8];
    enum class FRAME_BUFFER { FOG_1, FOG_2, BLOOM };

    Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffers[8];
    enum class CONSTANT_BUFFER { SCENE, FOG, MASK, PBR, D_FOG };
    enum class CONSTANT_BUFFER_R { NONE, SCENE, FOG, MASK, PBR, D_FOG, LIGHT/*register用*/ };

    std::unique_ptr<fullscreen_quad> bit_block_transfer[3];
    enum class BIT_BLOCK { FOG, BLOOM, SKY };

    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[8];
    enum class PS { FOG, FINAL, SKY, BLOOM };

    // dissolve
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> noise;

    // PBR
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> BaseColor;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Roughness;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Normal;

    // bloom
    std::unique_ptr<bloom> bloomer;

    // skyMap
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skymap;



    // MASK
    struct dissolve_constants {
        DirectX::XMFLOAT4 parameters; // x : ディゾルブ適応量、yzw : 空き
    };
    float dissolve_value{ 1.0f };



    // バッファー＝データを一時的に保持するメモリストレージ領域
    // ・プログラム側で定数バッファに値を格納することで
    // 　シェーダー内でプログラム側のパラメータを参照できる
    // ・ワールド行列、ビュー行列、プロジェクション行列などの
    // 　パラメータを定数バッファに書き込むことで、
    // 　シェーダー内で座標変換の計算ができる
    struct scene_constants
    {
        DirectX::XMFLOAT4X4 view_projection;
        DirectX::XMFLOAT4 light_direction;
        // UNIT.16
        DirectX::XMFLOAT4 camera_position;
        // FOG
        DirectX::XMFLOAT4X4 inverse_projection;
        float time;
        float pads[3];
        // SKYMAP
        DirectX::XMFLOAT4X4 inv_view_projection;
        // SHADOW
        DirectX::XMFLOAT4X4 light_view_projection;
        float shadow_depth_bias;
        float pads2[3];
    };
    scene_constants scene_constants;

    // FOG
    struct fog_constants
    {
        //float fog_color[4] = { 0.322f, 0.765f, 0.882f, 0.894f }; // w: fog intensuty
        float fog_color[4] = { 0.734f, 0.734f, 0.734f, 0.894f }; // w: fog intensuty
        float fog_density =0.15f;
        float fog_height_falloff = 0.8126f;
        float start_distance = 5.00f;
        float fog_cutoff_distance = 500.0f;
        float time_scale = 0.4f;
        float seed_scale = 0.03f;
        float pads[2];
    };
    fog_constants fog_constants;

    // STATIC_BATCHING
    size_t drawcall_count = 0;
    std::unique_ptr<static_mesh_batch>  static_meshes[8];

    /// <summary>
    /// ライト構造体
    /// </summary>
    struct light_constants
    {
        // ディレクションライト用のメンバ
        DirectX::XMFLOAT4 dirDirection;   // ライトの方向
        DirectX::XMFLOAT4 dirColor;       // ライトのカラー

        // step-1 ライト構造体にポイントライト用のメンバ変数を追加する
        DirectX::XMFLOAT4 ptPosition;
        DirectX::XMFLOAT4 ptColor;
        DirectX::XMFLOAT4 ptRange;

        DirectX::XMFLOAT4 eyePos;         // 視点の位置
        DirectX::XMFLOAT4 ambientLight;   // アンビエントライト
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> light_constant_buffer;
    // ライトのデータを作成する
    light_constants light;

    // PBR
    struct adjust_constants
    {
        DirectX::XMFLOAT4 materialColor;
        DirectX::XMFLOAT4 adjustMetalness = { 0.0f,0,0,0 }; // メタルネスの調整値
        DirectX::XMFLOAT4 adjustSmoothness = { 0.0f,0,0,0 }; // 滑らかさの調整値
    };
    adjust_constants pbr;

    // 距離フォグ
    struct dis_fog_constants
    {
        DirectX::XMFLOAT4 fog_color;
        DirectX::XMFLOAT4 fog_range; // x : near, y : far, zw : 空き
    };
    DirectX::XMFLOAT4 fog_color{ 0.2f, 0.2f, 0.2f, 1.0f }; // クリアカラーにしています
    DirectX::XMFLOAT4 fog_range{0.1f, 100.0f, 0, 0};

    // SHADOW
    const uint32_t shadowmap_width = 2048;
    const uint32_t shadowmap_height = 2048;
    std::unique_ptr<shadow_map> double_speed_z;
    DirectX::XMFLOAT4 light_view_focus{ 0, 0, 0, 1 };
    float light_view_distance{ 10.0f };
    float light_view_size{ 12.0f };
    float light_view_near_z{ 2.0f };
    float light_view_far_z{ 18.0f };

    /*----------------- チュートリアル -----------------*/

    std::unique_ptr<sprite>tutorial_01;
    std::unique_ptr<sprite>tutorial_02;
    std::unique_ptr<sprite>tutorial_03;

    // チュートリアルが終わったか
    std::array<bool, 3> tutorial_end;
    //チュートリアルの進行度
    int tutorial_i = 0;
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
