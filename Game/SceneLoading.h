#pragma once
#include"./Lemur/Scene/BaseScene.h"
#include"./Lemur/Scene/SceneManager.h"
#include"./Lemur/Graphics/shader.h"
#include"./Lemur/Graphics/texture.h"
#include"./Lemur/Graphics/framework.h"
#include <thread>

// ���[�f�B���O�V�[��
class SceneLoading: public Lemur::Scene::BaseScene
{
public:
    SceneLoading(BaseScene* nextScene) :nextScene(nextScene) {}
    SceneLoading() {}
    ~SceneLoading()override {}

    // ������
    void Initialize()override;

    // �I����
    void Finalize()override;

    // �X�V����
    void Update(HWND hwnd, float elapsedTime)override;

    // �`�揈��
    void Render(float elapsedTime)override;

private:
    // ���[�f�B���O�X���b�h
    static void LoadingThread(SceneLoading* scene);

private:
    float angle = 0.0f;
    BaseScene* nextScene = nullptr;
    std::thread* thread = nullptr;
    std::shared_ptr<sprite> sprLoading;
    std::shared_ptr<sprite> sprblack;

//----------------------------------------------------------------------------------------------------
//  ���V�F�[�_�[�֘A
//----------------------------------------------------------------------------------------------------
    std::unique_ptr<framebuffer> framebuffers[8];



    struct scene_constants
    {// ���̒l�̈ʒu�̓V�F�[�_�[���ƈ�v������
        DirectX::XMFLOAT4X4 view_projection; // �r���[�E�v���W�F�N�V�����ϊ��s�� 
        DirectX::XMFLOAT4 light_direction; // ���C�g�̌���
        DirectX::XMFLOAT4 camera_position; // ���C�g�̌���
        // SKYMAP
        DirectX::XMFLOAT4X4 inv_view_projection;
        // SHADOW
        DirectX::XMFLOAT4X4 light_view_projection;
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffers[8];

    // Audio
    //Microsoft::WRL::ComPtr<IXAudio2> xaudio2;
    //IXAudio2MasteringVoice* master_voice = nullptr;
    //std::unique_ptr<Lemur::Audio::audio> bgm[8];
    //std::unique_ptr<Lemur::Audio::audio> se[8];

    // Zelda_Shader
    Microsoft::WRL::ComPtr<ID3D11PixelShader> zelda_ps;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> stage_ps;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> chara_ps;

    // SKYMAP
    std::unique_ptr<fullscreen_quad> bit_block_transfer_sky;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skymap;


    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[8];
    // BLOOM
    std::unique_ptr<fullscreen_quad> bit_block_transfer;
    std::unique_ptr<bloom> bloomer;

    // MASK
    struct dissolve_constants {
        DirectX::XMFLOAT4 parameters; // x : �f�B�]���u�K���ʁAyzw : ��
    };
    float dissolve_value{ 0.5f };
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
    float light_view_size{ 30.0f };
    float light_view_near_z{ 1.0f };
    float light_view_far_z{ 100.0f };
};
