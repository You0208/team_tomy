#pragma once
#include <xaudio2.h>

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Lemur/Audio/audio.h"
#include "Lemur/Component/DemoEnemy.h"
#include "Lemur/Component/DemoPlayer.h"
#include "Lemur/Scene/BaseScene.h"

class GameScene :public Lemur::Scene::BaseScene
{
public:
    float x;
    float xp;

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

    Enemy* CreateEnemy()
    {
        return new Enemy(
            new EnemyInputComponent(),
            new EnemyPhysicsComponent(),
            new EnemyGraphicsComponent()
        );
    }


private:
    // 更新処理止める用
    bool is_update = true;

    //ポーズ
    void Pause()
    {
        if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_RIGHT_THUMB)
            is_update = !is_update;
    }

private:

    float timer;

    //DemoPlayer
    Player* player = nullptr;
    DemoEnemy* enemy = nullptr;

    std::unique_ptr<framebuffer> framebuffers[8];

    // Audio
    Microsoft::WRL::ComPtr<IXAudio2> xaudio2;
    IXAudio2MasteringVoice* master_voice = nullptr;
    std::unique_ptr<Lemur::Audio::audio> bgm[8];
    std::unique_ptr<Lemur::Audio::audio> se[8];

    // skkind
    std::shared_ptr<skinned_mesh> skinned_meshes[8];

    // Zelda_Shader
    Microsoft::WRL::ComPtr<ID3D11PixelShader> zelda_ps;

    // SKYMAP
    std::unique_ptr<fullscreen_quad> bit_block_transfer_sky;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skymap;


    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[8];
    // BLOOM
    std::unique_ptr<fullscreen_quad> bit_block_transfer;
    std::unique_ptr<bloom> bloomer;

    // MASK
    struct dissolve_constants {
        DirectX::XMFLOAT4 parameters; // x : ディゾルブ適応量、yzw : 空き
    };
    float dissolve_value{ 0.5f };
    Microsoft::WRL::ComPtr<ID3D11Buffer> dissolve_constant_buffer;

    D3D11_TEXTURE2D_DESC mask_texture2dDesc{};
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mask_texture;
    std::shared_ptr<sprite> dummy_sprite;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> sprite_vertex_shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> sprite_input_layout;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> sprite_pixel_shader;

    // SHADOW
    const uint32_t shadowmap_width = 2048;
    const uint32_t shadowmap_height = 2048;
    std::unique_ptr<shadow_map> double_speed_z;
    DirectX::XMFLOAT4 light_view_focus{ 0, 0, 0, 1 };
    float light_view_distance{ 10.0f };
    float light_view_size{ 12.0f };
    float light_view_near_z{ 2.0f };
    float light_view_far_z{ 18.0f };

    // バッファー＝データを一時的に保持するメモリストレージ領域
    // ・プログラム側で定数バッファに値を格納することで
    // 　シェーダー内でプログラム側のパラメータを参照できる
    // ・ワールド行列、ビュー行列、プロジェクション行列などの
    // 　パラメータを定数バッファに書き込むことで、
    // 　シェーダー内で座標変換の計算ができる
    struct scene_constants
    {// 中の値の位置はシェーダー側と一致させる
        DirectX::XMFLOAT4X4 view_projection; // ビュー・プロジェクション変換行列 
        DirectX::XMFLOAT4 light_direction; // ライトの向き
        DirectX::XMFLOAT4 camera_position; // ライトの向き
        // SKYMAP
        DirectX::XMFLOAT4X4 inv_view_projection;
        // SHADOW
        DirectX::XMFLOAT4X4 light_view_projection;

    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffers[8];

    DirectX::XMFLOAT3 cobe_position{ 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT4 camera_position{ 0.0f, 0.0f, -10.0f, 1.0f };

    DirectX::XMFLOAT4 light_direction{ -0.113f, -0.556f, 1.0f, 0.0f };

    DirectX::XMFLOAT3 translation{ 0, 0, 0 };
    DirectX::XMFLOAT3 scaling{ 1, 1, 1 };
    DirectX::XMFLOAT3 rotation{ 0, 0, 0 };
    DirectX::XMFLOAT4 material_color{ 1, 1, 1, 1 };
};
