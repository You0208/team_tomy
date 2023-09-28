#include "BaseScene.h"

void Lemur::Scene::BaseScene::SetState()
{
	Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
	HRESULT hr{ S_OK };

	// �[�x�X�e���V���X�e�[�g�I�u�W�F�N�g���쐬����
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
		depth_stencil_desc.DepthEnable = TRUE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = graphics.GetDevice()->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		depth_stencil_desc.DepthEnable = TRUE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = graphics.GetDevice()->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_OFF)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		depth_stencil_desc.DepthEnable = FALSE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = graphics.GetDevice()->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_ON)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		depth_stencil_desc.DepthEnable = FALSE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = graphics.GetDevice()->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// �u�����f�B���O�X�e�[�g�I�u�W�F�N�g���쐬����
	{
		D3D11_BLEND_DESC blend_desc{};
		blend_desc.AlphaToCoverageEnable = FALSE;
		blend_desc.IndependentBlendEnable = FALSE;
		blend_desc.RenderTarget[0].BlendEnable = FALSE;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = graphics.GetDevice()->CreateBlendState(&blend_desc, blend_states[static_cast<size_t>(BLEND_STATE::NONE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		blend_desc.AlphaToCoverageEnable = FALSE;
		blend_desc.IndependentBlendEnable = FALSE;
		blend_desc.RenderTarget[0].BlendEnable = TRUE;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = graphics.GetDevice()->CreateBlendState(&blend_desc, blend_states[static_cast<size_t>(BLEND_STATE::ALPHA)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		blend_desc.AlphaToCoverageEnable = FALSE;
		blend_desc.IndependentBlendEnable = FALSE;
		blend_desc.RenderTarget[0].BlendEnable = TRUE;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; //D3D11_BLEND_ONE D3D11_BLEND_SRC_ALPHA
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = graphics.GetDevice()->CreateBlendState(&blend_desc, blend_states[static_cast<size_t>(BLEND_STATE::ADD)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		blend_desc.AlphaToCoverageEnable = FALSE;
		blend_desc.IndependentBlendEnable = FALSE;
		blend_desc.RenderTarget[0].BlendEnable = TRUE;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO; //D3D11_BLEND_DEST_COLOR
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR; //D3D11_BLEND_SRC_COLOR
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = graphics.GetDevice()->CreateBlendState(&blend_desc, blend_states[static_cast<size_t>(BLEND_STATE::MULTIPLY)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
	// ���X�^���C�U�X�e�[�g�I�u�W�F�N�g�𐶐�(�O�̃X�e�[�W�ŏ������ꂽ�_��O�p�`�Ȃǂ���ʂ̃s�N�Z���ɕϊ�����)
	{
		D3D11_RASTERIZER_DESC rasterizer_desc{};
		rasterizer_desc.FillMode = D3D11_FILL_SOLID;
		rasterizer_desc.CullMode = D3D11_CULL_BACK;// �w���Ȃ����
		// ����n�E��n�ł�����ς���
		rasterizer_desc.FrontCounterClockwise = TRUE;// �����v���iCCW�j��O�ʂƂ���iCW�����v���j
		rasterizer_desc.DepthBias = 0;
		rasterizer_desc.DepthBiasClamp = 0;
		rasterizer_desc.SlopeScaledDepthBias = 0;
		rasterizer_desc.DepthClipEnable = TRUE;
		rasterizer_desc.ScissorEnable = FALSE;
		rasterizer_desc.MultisampleEnable = FALSE;
		rasterizer_desc.AntialiasedLineEnable = FALSE;
		hr = graphics.GetDevice()->CreateRasterizerState(&rasterizer_desc, rasterizer_states[static_cast<size_t>(RASTER_STATE::SOLID)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizer_desc.CullMode = D3D11_CULL_BACK;
		rasterizer_desc.AntialiasedLineEnable = TRUE;
		hr = graphics.GetDevice()->CreateRasterizerState(&rasterizer_desc, rasterizer_states[static_cast<size_t>(RASTER_STATE::WIREFRAME)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		rasterizer_desc.FillMode = D3D11_FILL_SOLID;
		rasterizer_desc.CullMode = D3D11_CULL_NONE;
		rasterizer_desc.AntialiasedLineEnable = TRUE;
		hr = graphics.GetDevice()->CreateRasterizerState(&rasterizer_desc, rasterizer_states[static_cast<size_t>(RASTER_STATE::CULL_NONE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizer_desc.CullMode = D3D11_CULL_NONE;
		rasterizer_desc.AntialiasedLineEnable = TRUE;
		hr = graphics.GetDevice()->CreateRasterizerState(&rasterizer_desc, rasterizer_states[static_cast<size_t>(RASTER_STATE::WIREFRAME_CULL_NONE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
	// �T���v���[�X�e�[�g�I�u�W�F�N�g�𐶐�����
	{
		D3D11_SAMPLER_DESC sampler_desc;
		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;// �e�N�X�`���̃T���v�����O���Ɏg�p����t�B���^�[�������@
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;// 0 ���� 1 �͈̔͊O�� u �e�N�X�`�����W���������邽�߂Ɏg�p���郁�\�b�h
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;// 0 ���� 1 �͈̔͊O�� v �e�N�X�`�����W���������邽�߂Ɏg�p���郁�\�b�h
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;// 0 ���� 1 �͈̔͊O�� w �e�N�X�`�����W���������邽�߂Ɏg�p���郁�\�b�h
		sampler_desc.MipLODBias = 0;// �v�Z���ꂽ�~�b�v�}�b�v ���x������̃I�t�Z�b�g
		sampler_desc.MaxAnisotropy = 16;// �L���Ȓl�� 1 �` 16
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;// �T���v�����O���ꂽ�f�[�^�������̃T���v�����O���ꂽ�f�[�^�Ɣ�r����֐�
		sampler_desc.BorderColor[0] = 0;// ���E���̐F
		sampler_desc.BorderColor[1] = 0;
		sampler_desc.BorderColor[2] = 0;
		sampler_desc.BorderColor[3] = 0;
		sampler_desc.MinLOD = 0;// �A�N�Z�X���N�����v����~�b�v�}�b�v�͈͂̉��[(0�������΂�ڍ�)
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;// �A�N�Z�X���N�����v����~�b�v�}�b�v�͈͂̏��
		hr = graphics.GetDevice()->CreateSamplerState(&sampler_desc, sampler_states[static_cast<size_t>(SAMPLER_STATE::POINT)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		hr = graphics.GetDevice()->CreateSamplerState(&sampler_desc, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		hr = graphics.GetDevice()->CreateSamplerState(&sampler_desc, sampler_states[static_cast<size_t>(SAMPLER_STATE::ANISOTROPIC)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		sampler_desc.BorderColor[0] = 0;
		sampler_desc.BorderColor[1] = 0;
		sampler_desc.BorderColor[2] = 0;
		sampler_desc.BorderColor[3] = 0;
		hr = graphics.GetDevice()->CreateSamplerState(&sampler_desc, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_BLACK)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		
		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		sampler_desc.BorderColor[0] = 1;
		sampler_desc.BorderColor[1] = 1;
		sampler_desc.BorderColor[2] = 1;
		sampler_desc.BorderColor[3] = 1;
		hr = graphics.GetDevice()->CreateSamplerState(&sampler_desc, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR_BORDER_WHITE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		
		// SHADOW
		sampler_desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL; //D3D11_COMPARISON_LESS_EQUAL
		sampler_desc.BorderColor[0] = 1;
		sampler_desc.BorderColor[1] = 1;
		sampler_desc.BorderColor[2] = 1;
		sampler_desc.BorderColor[3] = 1;
		hr = graphics.GetDevice()->CreateSamplerState(&sampler_desc, sampler_states[static_cast<size_t>(SAMPLER_STATE::COMPARISON_LINEAR_BORDER_WHITE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

}
