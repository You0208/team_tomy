#include "../Graphics/Graphics.h"
#include "EffekseerManager.h"

// ������
void EffectManager::Initialize()
{
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();

    // Effekseer�����_������
    effekseerRenderer = EffekseerRendererDX11::Renderer::Create(graphics.GetDevice(),
        graphics.GetDeviceContext(), 2048);

    // Effekseer�}�l�[�W���[����
    effekseerManager = Effekseer::Manager::Create(2048);

    //Effekseer�����_���̊e��ݒ�i���ʂȃJ�X�^�}�C�Y�����Ȃ��ꍇ�͒�^�I�Ɉȉ��̐ݒ��OK�j
    effekseerManager->SetSpriteRenderer(effekseerRenderer->CreateSpriteRenderer());
    effekseerManager->SetRibbonRenderer(effekseerRenderer->CreateRibbonRenderer());
    effekseerManager->SetRingRenderer(effekseerRenderer->CreateRingRenderer());
    effekseerManager->SetTrackRenderer(effekseerRenderer->CreateTrackRenderer());
    effekseerManager->SetModelRenderer(effekseerRenderer->CreateModelRenderer());
    // Effekseer���ł̃��[�_�[�̐ݒ�i���ʂȃJ�X�^�}�C�Y�����Ȃ��ꍇ�͈ȉ��̐ݒ��OK�j
    effekseerManager->SetTextureLoader(effekseerRenderer->CreateTextureLoader());
    effekseerManager->SetModelLoader(effekseerRenderer->CreateModelLoader());
    effekseerManager->SetMaterialLoader(effekseerRenderer->CreateMaterialLoader());

    //Effekseer��������W�n�Ōv�Z����
    effekseerManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

// �I����
void EffectManager::Finalize()
{
    // EffekseerManager�Ȃǂ̓X�}�[�g�|�C���^�ɂ���Ĕj�������̂ŉ������Ȃ�
}

// �X�V����
void EffectManager::Update(float elapsedTime)
{
    //�G�t�F�N�g�̍X�V����
    effekseerManager->Update(elapsedTime * 60.0f);
}

// �`��X�V
void EffectManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    // �r���[���v���W�F�N�V�����s��wpEffekseer�����_���ɐݒ�
    effekseerRenderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    effekseerRenderer->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&projection));

    // Effekseer�`��J�n
    effekseerRenderer->BeginRendering();

    effekseerManager->Draw();

    effekseerRenderer->EndRendering();
}