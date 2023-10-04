#pragma once
#include "BaseScene.h"
#include "../Component/GameObject.h"
#include "../Component/DemoPlayer.h"
#include "../Component/DemoEnemy.h"
#include "../Graphics/shader.h"
#include "../Graphics/texture.h"
#include "../Graphics/framework.h"

// BLOOM
#include "../Graphics/bloom.h"

// Audio
#include <wrl.h>
#include "../Audio/audio.h"

#include "../Effekseer/Effect.h"

// Font
#include"../Font/Font.h"
#include <dwrite.h>                
#pragma comment(lib,"d2d1.lib")    
#pragma comment(lib,"dwrite.lib")


class DemoScene :public Lemur::Scene::BaseScene
{
public:
    DemoScene() {}
    ~DemoScene() override {}

    // 初期化
    void Initialize()override;

    // 終了化
    void Finalize()override;

    // 更新処理
    void Update(HWND hwnd, float elapsedTime)override;

    // 描画処理
    void Render(float elapsedTime)override;

    //HACK ユニークポインタ
    DemoPlayer* CreatePlayer()
    {
        return new DemoPlayer(
            new DemoPlayerInputComponent(),
            new DemoPlayerPhysicsComponent(),
            new DemoPlayerGraphicsComponent()
        );
    }

    DemoEnemy* CreateEnemy()
    {
        return new DemoEnemy(
            new DemoEnemyInputComponent(),
            new DemoEnemyPhysicsComponent(),
            new DemoEnemyGraphicsComponent()
        );
    }
private:
    std::unique_ptr<framebuffer> framebuffers[8];

    // dissolve
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> noise;

    // PBR
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> BaseColor;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Roughness;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Normal;

    // SKYMAP
    //std::unique_ptr<fullscreen_quad> bit_block_transfer_sky;
    //Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skymap;

    //Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[8];
    // BLOOM
    //std::unique_ptr<fullscreen_quad> bit_block_transfer;
    //std::unique_ptr<bloom> bloomer;

    // bloom
    std::unique_ptr<fullscreen_quad> bitBlockTransfer;
    std::unique_ptr<bloom> bloomer;

    // skyMap
    std::unique_ptr<fullscreen_quad> bitBlockTransfer_sky;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skymap;


    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[8];

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


    // Audio
    Microsoft::WRL::ComPtr<IXAudio2> xaudio2;
    IXAudio2MasteringVoice* master_voice = nullptr;
    std::unique_ptr<Lemur::Audio::audio> bgm[8];
    std::unique_ptr<Lemur::Audio::audio> se[8];

    //DemoPlayer
    DemoPlayer* player = nullptr;
    DemoEnemy* enemy = nullptr;

    float second;

    // skkind
    std::shared_ptr<skinned_mesh> skinned_meshes[8];

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

    //TODO PBR
    struct adjust_constants
    {
        DirectX::XMFLOAT4 materialColor;
        DirectX::XMFLOAT4 adjustMetalness = { 1.0f,0,0,0 }; // メタルネスの調整値
        DirectX::XMFLOAT4 adjustSmoothness = {1.0f,0,0,0}; // 滑らかさの調整値
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> adjust_constant_buffer;
    adjust_constants adjust;

    std::shared_ptr<skinned_mesh> cube;


    DirectX::XMFLOAT4 camera_position{ 0.0f, 0.0f, -10.0f, 1.0f };
    DirectX::XMFLOAT4 light_direction{ -0.113f, -0.556f, 1.0f, 0.0f };

    DirectX::XMFLOAT3 translation{ 0, 0, 0 };
    DirectX::XMFLOAT3 scaling{ 1, 1, 1 };
    DirectX::XMFLOAT3 rotation{ 0, 0, 0 };
    DirectX::XMFLOAT4 material_color{ 1, 1, 1, 1 };
};