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

    // ������
    void Initialize()override;

    // �I����
    void Finalize()override;

    // �X�V����
    void Update(HWND hwnd, float elapsedTime)override;

    // �`�揈��
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


private:// �Q�[���֘A

    // �G�𐶂ݏo��(�֐����͉�)
    //�I�����ꂽ�N�G�X�g�ɂ���Đ��ݏo���G���ς��
    void CreateEnemy_KARI();

    // �G��S���|������V�[���؂�ւ�
    void QuestClear();
    // �X�V�����~�߂�p
    bool is_update = true;

    // �������ɂ������X�e�[�^�X�̑�����{��
    float bet_rate = 1.0f;

    //�|�[�Y
    void Pause()
    {
        if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_RIGHT_THUMB)
            is_update = !is_update;
    }



    float timer;

    //DemoPlayer
    Player* player = nullptr;
    DemoEnemy* enemy = nullptr;

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

    DirectX::XMFLOAT4 light_direction{ -0.113f, -0.556f, 1.0f, 0.0f };

    DirectX::XMFLOAT3 translation{ 0, 0, 0 };
    DirectX::XMFLOAT3 scaling{ 1, 1, 1 };
    DirectX::XMFLOAT3 rotation{ 0, 0, 0 };
    DirectX::XMFLOAT4 material_color{ 1, 1, 1, 1 };

private:// �V�F�[�_�[�֘A
    std::unique_ptr<framebuffer> framebuffers[8];
    enum class FRAME_BUFFER { FOG_1, FOG_2, BLOOM };

    Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffers[8];
    enum class CONSTANT_BUFFER { SCENE, FOG, MASK, PBR, D_FOG };
    enum class CONSTANT_BUFFER_R { NONE, SCENE, FOG, MASK, PBR, D_FOG, LIGHT/*register�p*/ };

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
        DirectX::XMFLOAT4 parameters; // x : �f�B�]���u�K���ʁAyzw : ��
    };
    float dissolve_value{ 1.0f };



    // �o�b�t�@�[���f�[�^���ꎞ�I�ɕێ����郁�����X�g���[�W�̈�
    // �E�v���O�������Œ萔�o�b�t�@�ɒl���i�[���邱�Ƃ�
    // �@�V�F�[�_�[���Ńv���O�������̃p�����[�^���Q�Ƃł���
    // �E���[���h�s��A�r���[�s��A�v���W�F�N�V�����s��Ȃǂ�
    // �@�p�����[�^��萔�o�b�t�@�ɏ������ނ��ƂŁA
    // �@�V�F�[�_�[���ō��W�ϊ��̌v�Z���ł���
    struct scene_constants
    {
        DirectX::XMFLOAT4X4 view_projection;
        DirectX::XMFLOAT4 light_direction;
        // UNIT.16
        DirectX::XMFLOAT4 camera_position;
        // FOG
        DirectX::XMFLOAT4X4 inverse_projection;
        DirectX::XMFLOAT4X4 inverse_view_projection;
        float time;
        float pads[3];
        // SKYMAP
        DirectX::XMFLOAT4X4 inv_view_projection;
        // SHADOW
        DirectX::XMFLOAT4X4 light_view_projection;
    };
    scene_constants scene_constants;

    // FOG
    struct fog_constants
    {
        //float fog_color[4] = { 0.322f, 0.765f, 0.882f, 0.894f }; // w: fog intensuty
        float fog_color[4] = { 1.000f, 1.000f, 1.000f, 0.894f }; // w: fog intensuty
        float fog_density = 0.0007f;
        float fog_height_falloff = 0.9313f;
        float start_distance = 5.00f;
        float fog_cutoff_distance = 500.0f;
        float time_scale = 0.5f;
        float seed_scale = 0.2f;
        float pads[2];
    };
    fog_constants fog_constants;

    // STATIC_BATCHING
    size_t drawcall_count = 0;
    std::unique_ptr<static_mesh_batch>  static_meshes[8];

    /// <summary>
    /// ���C�g�\����
    /// </summary>
    struct light_constants
    {
        // �f�B���N�V�������C�g�p�̃����o
        DirectX::XMFLOAT4 dirDirection;   // ���C�g�̕���
        DirectX::XMFLOAT4 dirColor;       // ���C�g�̃J���[

        // step-1 ���C�g�\���̂Ƀ|�C���g���C�g�p�̃����o�ϐ���ǉ�����
        DirectX::XMFLOAT4 ptPosition;
        DirectX::XMFLOAT4 ptColor;
        DirectX::XMFLOAT4 ptRange;

        DirectX::XMFLOAT4 eyePos;         // ���_�̈ʒu
        DirectX::XMFLOAT4 ambientLight;   // �A���r�G���g���C�g
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> light_constant_buffer;
    // ���C�g�̃f�[�^���쐬����
    light_constants light;

    // PBR
    struct adjust_constants
    {
        DirectX::XMFLOAT4 materialColor;
        DirectX::XMFLOAT4 adjustMetalness = { 0.0f,0,0,0 }; // ���^���l�X�̒����l
        DirectX::XMFLOAT4 adjustSmoothness = { 0.0f,0,0,0 }; // ���炩���̒����l
    };
    adjust_constants pbr;

    // �����t�H�O
    struct dis_fog_constants
    {
        DirectX::XMFLOAT4 fog_color;
        DirectX::XMFLOAT4 fog_range; // x : near, y : far, zw : ��
    };
    DirectX::XMFLOAT4 fog_color{ 0.2f, 0.2f, 0.2f, 1.0f }; // �N���A�J���[�ɂ��Ă��܂�
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
};
