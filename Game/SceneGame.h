#pragma once
#include"./Lemur/Scene/BaseScene.h"
#include"./Lemur/Graphics/shader.h"
#include"./Lemur/Graphics/texture.h"
#include"./Lemur/Graphics/framework.h"

// Player
#include "Player.h"

// Stage
#include "Stage.h"

//Enemy
#include "Enemy.h"

//Ui
#include "Ui.h"

// Projectile
#include "GamePro_ProjectileStraight.h"
//class GamePro_ProjectileStraight;
// Audio
#include <wrl.h>
#include"./Lemur/Audio/audio.h"

// Effect
#include"./Lemur/Effekseer/Effect.h"

// Audio
#include <wrl.h>
#include "../Lemur/Audio/audio.h"


#include<vector>
#include<algorithm>
#include <iostream>



class SceneGame :public Lemur::Scene::BaseScene
{
public:
    SceneGame() {}
    ~SceneGame() override {}

    // ������
    void Initialize()override;

    // �I����
    void Finalize()override;

    // �X�V����
    void Update(HWND hwnd,float elapsedTime)override;

    // �`�揈��
    void Render(float elapsedTime)override;

    // �G�̒ǉ�
    void addEnemy();
    void addEnemy(int enemyType,int startTime);

    //Ui�̒ǉ�
    void addUi(int Uitype);
    void UiGetUpdate();
    void EnemyGetUpdate();

    bool JudgeState(int i);

    // �e�̒ǉ�
    void addProjectile();

    void ProjectileVSEnemy();

    void Wave();

    // �v���C���[����
    Player* CreatePlayer()
    {
        return new Player(
            new PlayerInputComponent(),
            new PlayerPhysicsComponent(),
            new PlayerGraphicsComponent()
        );
    }

    Stage* CreateStage()
    {
        return new Stage(
            new StageInputComponent(),
            new StagePhysicsComponent(),
            new StageGraphicsComponent()
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

    Ui* CreateUi()
    {
        return new Ui(
            new UiInputComponent(),
            new UiPhysicsComponent(),
            new UiGraphicsComponent()
        );
    }

    static int Timer;
    //GamePro_ProjectileStraight* CreateProjectile();
    GamePro_ProjectileStraight* CreateProjectile()
    {
        return new GamePro_ProjectileStraight(
            new GamePro_ProjectileStraightInputComponent(),
            new GamePro_ProjectileStraightPhysicsComponent(),
            new GamePro_ProjectileStraightGraphicsComponent()
        );
    }

   

    int ShopItemsNum[10] = {};
    int SaveShopUi = {};
    int shop_int;
    int UiCount = {};
    int jank = 0;

    bool isPaused = false;

    bool hit=false;



    //�X�e�[�^�X
    float speed = 0.005f;
    float damage = 5.0f;
    float attack = 1.0f;
    float attacktimer = 0;
    float HP = 1.0f;
    float Player_HP = 3.0f;
    float Player_MAXHP = 3.0f;

    // �E�F�[�u�p
    bool SetPhase = false;
    int WaveNumber=1;
    //std::vector<GameObject> waves;
    // 
    //�X�e�[�^�X���x��
    int speed_lv;
    int damage_lv;
    int attack_lv;
    int HP_lv;
    int Player_MAXHP_Lv;
    int speed_MAXlv;
    int damage_MAXlv;
    int attack_MAXlv;
    int HP_MAXlv;
    int Player_MAXHP_MAXLv;
    //
    bool player_dead;
    //BGM�ESE
    Microsoft::WRL::ComPtr<IXAudio2> xaudio2;
    IXAudio2MasteringVoice* master_voice = nullptr;
    std::unique_ptr<Lemur::Audio::audio> shot;
    std::unique_ptr<Lemur::Audio::audio> purchase;
    std::unique_ptr<Lemur::Audio::audio> explosion;
    std::unique_ptr<Lemur::Audio::audio> result;

    enum class enemyNum
    {
        EnemyPlayerDeath_Flag,
        EnemyPlayerDeath_Call,
        EnemyMomey,
        EnemyDeath_Flag,
        EnemyDeath_Call,
        a5,
        a6,
        a7,
        a8,
        EnemyDamage
    };


    std::unique_ptr<Lemur::Audio::audio> damageSE;

    std::unique_ptr<Lemur::Audio::audio> BGM;
    std::unique_ptr<Lemur::Audio::audio> BGM2;
    std::unique_ptr<Lemur::Audio::audio> BGM3;

    bool DeathCheack;

    //Effect
    Effect* explosionEffect;
    Effect* barelEffect;

    DirectX::XMFLOAT3 TANI;
    DirectX::XMFLOAT3 TANI_Effect;
    DirectX::XMVECTOR tani;



private:
    // skkind_mesh
    std::shared_ptr<skinned_mesh> skinned_meshes[8];
    DirectX::XMFLOAT4 camera_position{ 0.0f, 0.0f, -10.0f, 1.0f };
    DirectX::XMFLOAT4 light_direction{ 0.048f, -0.538f, -0.51f, 0.0f };

    DirectX::XMFLOAT4 intersection_point{};
    //DirectX::XMFLOAT4X4 island_transform{ -0.01f, 0, 0, 0, 0, 0.01f, 0, 0, 0, 0, 0.01f, 0, 0, 0, 0, 1 };
    //scene_constants scene_data{};

    // Stage
    Stage* stage;

    // Player
    Player* player;

    //Enemy
    Enemy* enemy;


    //Ui
    GameObject* ui;
    std::vector<Enemy*> enemyList;
    std::vector<GameObject*> UiList;

    // Projectile
    GameObject* projectile;
    std::vector<GameObject*> projectileList;

    

    // �|�[�Y�i�V���b�v�j


    // �G�t�F�N�g
    Effect* hitEffect = nullptr;


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


