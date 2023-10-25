#pragma once

#include "../Graphics/Graphics.h"
#include <windows.h>
#include <sstream>
#include <d3d11.h>
#include <wrl.h>

namespace Lemur::Scene
{
    // �x�[�X�V�[��
    class BaseScene
    {
    public:
        BaseScene() {}
        virtual ~BaseScene() {}

        // ������
        virtual void Initialize() = 0;
        // �I����
        virtual void Finalize() = 0;
        // �X�V����
        virtual void Update(HWND hwnd, float elapsefTime) = 0;
        // �`�揈��
        virtual void Render(float elapsedTime) = 0;

        // �����������Ă��邩
        bool IsReady()const { return ready; }
        // ���������ݒ�
        void SetReady() { ready = true; }

    protected:
        enum class SAMPLER_STATE { POINT, LINEAR, ANISOTROPIC, LINEAR_BORDER_BLACK, LINEAR_BORDER_WHITE, COMPARISON_LINEAR_BORDER_WHITE, LINEAR_CLAMP };
        Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_states[7];

        enum  DEPTH_STATE { ZT_ON_ZW_ON, ZT_ON_ZW_OFF, ZT_OFF_ZW_ON, ZT_OFF_ZW_OFF };
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_states[4];

        enum  BLEND_STATE { NONE, ALPHA, ADD, MULTIPLY };
        Microsoft::WRL::ComPtr<ID3D11BlendState> blend_states[4];

        enum class RASTER_STATE { SOLID, WIREFRAME, CULL_NONE, WIREFRAME_CULL_NONE };
        Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_states[4];

        void SetState();

        void SetUpRendering();

    protected:
        D3D11_TEXTURE2D_DESC mask_texture2dDesc{};
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mask_texture;
        std::shared_ptr<sprite> dummy_sprite;
        Microsoft::WRL::ComPtr<ID3D11VertexShader> sprite_vertex_shader;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> sprite_input_layout;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> sprite_pixel_shader;

        // Zelda_Shader
        Microsoft::WRL::ComPtr<ID3D11PixelShader> zelda_ps;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> Wall;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> Try;

        bool enableShadow = true;
        bool enableSkyMap = false;
        bool enableBloom = false;
        bool enableFog = false;

    private:
        bool ready = false;
    };

}