#pragma once
#include"./Lemur/Scene/BaseScene.h"
#include"./Lemur/Scene/SceneManager.h"
#include"./Lemur/Graphics/shader.h"
#include"./Lemur/Graphics/texture.h"
#include"./Lemur/Graphics/framework.h"
#include"./Lemur/Graphics/sprite_d.h"

// Audio
#include <wrl.h>
#include"./Lemur/Audio/audio.h"

// Effect
#include"./Lemur/Effekseer/Effect.h"
#include <random>

class SceneTitle :public Lemur::Scene::BaseScene
{
public:
    SceneTitle() {}
    ~SceneTitle() override {}

    // 初期化
    void Initialize()override;

    // 終了化
    void Finalize()override;

    // 更新処理
    void Update(HWND hwnd, float elapsedTime)override;

    // 描画処理
    void Render(float elapsedTime)override;


    float color_[5];
private:
    // skkind_mesh
    std::shared_ptr<skinned_mesh> skinned_meshes[8];
    DirectX::XMFLOAT4 camera_position{ 0.0f, 0.0f, -10.0f, 1.0f };
    DirectX::XMFLOAT4 light_direction{ -0.113f, -0.556f, 1.0f, 0.0f };

    DirectX::XMFLOAT2 decoPosition[5];
    float decoSize[5];
    float decoW[5];
    

    std::shared_ptr<sprite_d> sprdissolve;

    std::shared_ptr<sprite> sprTitle;
    std::shared_ptr<sprite> sprStart;
    std::shared_ptr<sprite> sprEnd;
    std::shared_ptr<sprite> sprTitleDeco[5];

    struct option
    {
        DirectX::XMFLOAT2 size;
        DirectX::XMFLOAT2 pos;
    };

    option start;
    option end;

    bool dissolveStart;


    //-------------------------------------------------------------------------------------------
    //  ↓シェーダー関連
    //----------------------------------------------------------------------------------------------------
    std::unique_ptr<framebuffer> framebuffers[8];

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

    // Audio
    Microsoft::WRL::ComPtr<IXAudio2> xaudio2;
    IXAudio2MasteringVoice* master_voice = nullptr;
    std::unique_ptr<Lemur::Audio::audio> title;
    std::unique_ptr<Lemur::Audio::audio> bgm[8];
    std::unique_ptr<Lemur::Audio::audio> se[8];

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
    float dissolve_value{ 1.0f };
    Microsoft::WRL::ComPtr<ID3D11Buffer> dissolve_constant_buffer;

    D3D11_TEXTURE2D_DESC mask_texture2dDesc;
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
};