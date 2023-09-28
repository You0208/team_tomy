#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <cstdint>

// framebuffer �I�u�W�F�N�g���e�N�X�`���i�V�F�[�_�[���\�[�X�r���[�j�Ƃ��Ĉ����A��ʂɕ`�悷��
// �u�t���X�N���[���N�A�b�h�v�ɂ̓s�N�Z�� �V�F�[�_�ƃT���v���[�̏�Ԃ�����܂���B �s�N�Z�� �V�F�[�_�ƃT���v���[�̏�Ԃ������ō쐬���Đݒ肷��K�v������܂�
class fullscreen_quad
{
public:
	fullscreen_quad(ID3D11Device* device);
	virtual ~fullscreen_quad() = default;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> embedded_vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> embedded_pixel_shader;

public:
	void blit(ID3D11DeviceContext* immediate_contextbool, ID3D11ShaderResourceView** shader_resource_view, uint32_t start_slot, uint32_t num_views, ID3D11PixelShader* replaced_pixel_shader = nullptr);
};
