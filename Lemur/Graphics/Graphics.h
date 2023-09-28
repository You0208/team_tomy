#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>

#include "misc.h"
#include "sprite.h"
#include "sprite_batch.h"
#include "high_resolution_timer.h"
#include "geometric_primitive.h"
#include "static_mesh.h"
#include "skinned_mesh.h"
#include "framebuffer.h"
#include "fullscreen_quad.h"

// BLOOM
#include "bloom.h"

// SHADOW
#include "shadow_map.h"

// Debug
#include "DebugRenderer.h"

#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>
#include <dxgi1_6.h>

#define ENABLE_DIRECT2D
#ifdef ENABLE_DIRECT2D
#include <d2d1_1.h>
#include <dwrite.h>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#endif

#include <mutex>

CONST LONG SCREEN_WIDTH{ 1280 };
CONST LONG SCREEN_HEIGHT{ 720 };
CONST BOOL FULLSCREEN{ FALSE };

namespace Lemur::Graphics
{
    class Graphics
    {
    public:
        // fullscreen
        CONST HWND hwnd;
        BOOL fullscreen_mode{ FALSE };
        BOOL vsync{ TRUE };
        BOOL tearing_supported{ FALSE };
        SIZE framebuffer_dimensions;

        Microsoft::WRL::ComPtr<IDXGIAdapter3> adapter;
        size_t video_memory_usage()
        {
            DXGI_QUERY_VIDEO_MEMORY_INFO video_memory_info;
            adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &video_memory_info);
            return video_memory_info.CurrentUsage / 1024 / 1024;
        }
        void create_swap_chain(IDXGIFactory6* dxgi_factory6);

#ifdef ENABLE_DIRECT2D
        Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2d1_device_context;
        Microsoft::WRL::ComPtr<IDWriteTextFormat> dwrite_text_formats[8];
        Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> d2d_solid_color_brushes[8];
        void create_direct2d_objects();
#endif

        RECT windowed_rect;
        DWORD windowed_style;
        void fullscreen_state(BOOL fullscreen);
        void on_size_changed(UINT64 width, UINT height);


        void initialize(HWND hWnd, bool fullscreen);
        Graphics(HWND hwnd, bool fullscreen);
        virtual ~Graphics() = default;

        void render(float elapsed_time/*Elapsed seconds from last frame*/);

        static Graphics& Instance() { return *instance; }

        // デバイスの取得
        ID3D11Device* GetDevice() const { return device.Get(); }
        ID3D11DeviceContext* GetDeviceContext() const { return immediate_context.Get(); }
        ID3D11RenderTargetView* GetRenderTargetView() const { return render_target_view.Get(); }
        ID3D11DepthStencilView* GetDepthStencilView() const { return depth_stencil_view.Get(); }
        D3D11_TEXTURE2D_DESC* GetTexture2D() { return &texture2d_desc; }
        IDXGISwapChain1* GetSwap() const { return swap_chain.Get(); }

        // デバッグレンダラ取得
        DebugRenderer* GetDebugRenderer() const { return debugRenderer.get(); }

        // ミューテックス取得
        std::mutex& GetMutex() { return mutex; }

        std::unique_ptr<DebugRenderer>					debugRenderer;
    private:
        Microsoft::WRL::ComPtr<ID3D11Device> device;// DirectX11で利用する様々なリソースを作成するやつ
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context;// 様々な描画命令をGPUに伝えるやつ
        Microsoft::WRL::ComPtr<IDXGISwapChain1> swap_chain;// キャンバスに描いた画を額(ウインドウ)に入れるやつ
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;// 色を書き込むキャンバス
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;// 奥行き情報を書き込むキャンバス
       

        enum class SAMPLER_STATE { POINT, LINEAR, ANISOTROPIC };
        Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_states[3]; //どのようにテクスチャの色をサンプルする(取り出す) かの設定。

        enum  DEPTH_STATE { ZT_ON_ZW_ON, ZT_ON_ZW_OFF, ZT_OFF_ZW_ON, ZT_OFF_ZW_OFF };
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_states[4];

        enum  BLEND_STATE { NONE, ALPHA, ADD, MULTIPLY };
        Microsoft::WRL::ComPtr<ID3D11BlendState> blend_states[4];

        enum class RASTER_STATE { SOLID, WIREFRAME, CULL_NONE, WIREFRAME_CULL_NONE };
        Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_states[4];

        std::unique_ptr<sprite> sprites[8];
        std::unique_ptr<sprite_batch> sprite_batches[8];



        Microsoft::WRL::ComPtr<ID3D11PixelShader> replaced_pixel_shader;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> replaced_shader_resource_view;


        // static_mesh
        std::unique_ptr<static_mesh> static_meshes[8];


        Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[8];

        std::unique_ptr<framebuffer> framebuffers[8];

        std::unique_ptr<fullscreen_quad> bit_block_transfer;

        // BLOOM
        std::unique_ptr<bloom> bloomer;

        // SKYMAP
        D3D11_TEXTURE2D_DESC texture2d_desc{};

        float factors[4]{ 0.0f, 121.438332f };

        std::unique_ptr<geometric_primitive> geometric_primitives[8];

    private:
        static Graphics* instance;

        std::mutex mutex;
    };
}