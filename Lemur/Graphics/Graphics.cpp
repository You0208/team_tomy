#include "Graphics.h"
#include "shader.h"
#include "texture.h"
#include "Camera.h"
void acquire_high_performance_adapter(IDXGIFactory6* dxgi_factory6, IDXGIAdapter3** dxgi_adapter3)
{
	HRESULT hr{ S_OK };

	Microsoft::WRL::ComPtr<IDXGIAdapter3> enumerated_adapter;
	for (UINT adapter_index = 0; DXGI_ERROR_NOT_FOUND != dxgi_factory6->EnumAdapterByGpuPreference(adapter_index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(enumerated_adapter.ReleaseAndGetAddressOf())); ++adapter_index)
	{
		DXGI_ADAPTER_DESC1 adapter_desc;
		hr = enumerated_adapter->GetDesc1(&adapter_desc);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		if (adapter_desc.VendorId == 0x1002/*AMD*/ || adapter_desc.VendorId == 0x10DE/*NVIDIA*/)
		{
			OutputDebugStringW((std::wstring(adapter_desc.Description) + L" has been selected.\n").c_str());
			OutputDebugStringA(std::string("\tVendorId:" + std::to_string(adapter_desc.VendorId) + '\n').c_str());
			OutputDebugStringA(std::string("\tDeviceId:" + std::to_string(adapter_desc.DeviceId) + '\n').c_str());
			OutputDebugStringA(std::string("\tSubSysId:" + std::to_string(adapter_desc.SubSysId) + '\n').c_str());
			OutputDebugStringA(std::string("\tRevision:" + std::to_string(adapter_desc.Revision) + '\n').c_str());
			OutputDebugStringA(std::string("\tDedicatedVideoMemory:" + std::to_string(adapter_desc.DedicatedVideoMemory) + '\n').c_str());
			OutputDebugStringA(std::string("\tDedicatedSystemMemory:" + std::to_string(adapter_desc.DedicatedSystemMemory) + '\n').c_str());
			OutputDebugStringA(std::string("\tSharedSystemMemory:" + std::to_string(adapter_desc.SharedSystemMemory) + '\n').c_str());
			OutputDebugStringA(std::string("\tAdapterLuid.HighPart:" + std::to_string(adapter_desc.AdapterLuid.HighPart) + '\n').c_str());
			OutputDebugStringA(std::string("\tAdapterLuid.LowPart:" + std::to_string(adapter_desc.AdapterLuid.LowPart) + '\n').c_str());
			OutputDebugStringA(std::string("\tFlags:" + std::to_string(adapter_desc.Flags) + '\n').c_str());
			break;
		}
	}
	*dxgi_adapter3 = enumerated_adapter.Detach();
}

namespace Lemur::Graphics
{
	Graphics* Graphics::instance = nullptr;

	Graphics::Graphics(HWND hwnd, bool fullscreen) : hwnd(hwnd), fullscreen_mode(fullscreen), windowed_style(static_cast<DWORD>(GetWindowLongPtrW(hwnd, GWL_STYLE)))
	{
		// インスタンス設定
		instance = this;

		if (fullscreen)
		{
			fullscreen_state(TRUE);
		}

		RECT client_rect;
		GetClientRect(hwnd, &client_rect);
		framebuffer_dimensions.cx = client_rect.right - client_rect.left;
		framebuffer_dimensions.cy = client_rect.bottom - client_rect.top;

		HRESULT hr{ S_OK };

		UINT create_factory_flags{};
#ifdef _DEBUG
		create_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
		Microsoft::WRL::ComPtr<IDXGIFactory6> dxgi_factory6;
		hr = CreateDXGIFactory2(create_factory_flags, IID_PPV_ARGS(dxgi_factory6.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		acquire_high_performance_adapter(dxgi_factory6.Get(), adapter.GetAddressOf());

		UINT create_device_flags{ 0 };
#ifdef _DEBUG
		create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#ifdef ENABLE_DIRECT2D
		create_device_flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif
		D3D_FEATURE_LEVEL feature_levels{ D3D_FEATURE_LEVEL_11_1 };
		hr = D3D11CreateDevice(adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, 0, create_device_flags, &feature_levels, 1, D3D11_SDK_VERSION, &device, NULL, &immediate_context);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		create_swap_chain(dxgi_factory6.Get());
#ifdef ENABLE_DIRECT2D
		create_direct2d_objects();
#endif
		//--------------------------★追加↓--------------------------
		//// Direct2D,DirectWriteの初期化
		//hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
		//
		//hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(&g_pBackBuffer));
		//
		//FLOAT dpiX = SCREEN_WIDTH;
		//FLOAT dpiY = SCREEN_HEIGHT;
		//
		//D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);
		//
		//hr = g_pD2DFactory->CreateDxgiSurfaceRenderTarget(g_pBackBuffer, &props, &g_pRT);
		//
		//hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&g_pDWriteFactory));
		//
		////関数CreateTextFormat()
		////第1引数：フォント名（L"メイリオ", L"Arial", L"Meiryo UI"等）
		////第2引数：フォントコレクション（nullptr）
		////第3引数：フォントの太さ（DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT_BOLD等）
		////第4引数：フォントスタイル（DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STYLE_ITALIC）
		////第5引数：フォントの幅（DWRITE_FONT_STRETCH_NORMAL,DWRITE_FONT_STRETCH_EXTRA_EXPANDED等）
		////第6引数：フォントサイズ（20, 30等）
		////第7引数：ロケール名（L""）
		////第8引数：テキストフォーマット（&g_pTextFormat）
		//hr = g_pDWriteFactory->CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"", &g_pTextFormat);
		//
		////関数SetTextAlignment()
		////第1引数：テキストの配置（DWRITE_TEXT_ALIGNMENT_LEADING：前, DWRITE_TEXT_ALIGNMENT_TRAILING：後, DWRITE_TEXT_ALIGNMENT_CENTER：中央,
		////                         DWRITE_TEXT_ALIGNMENT_JUSTIFIED：行いっぱい）
		//hr = g_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		//
		////関数CreateSolidColorBrush()
		////第1引数：フォント色（D2D1::ColorF(D2D1::ColorF::Black)：黒, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))：RGBA指定）
		//hr = g_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::BlueViolet), &g_pSolidBrush);

		//--------------------------★追加↑--------------------------


		debugRenderer = std::make_unique<DebugRenderer>(device.Get());
	}

	void Graphics::create_swap_chain(IDXGIFactory6* dxgi_factory6)
	{
		HRESULT hr{ S_OK };

		if (swap_chain)
		{
			ID3D11RenderTargetView* null_render_target_view{};
			immediate_context->OMSetRenderTargets(1, &null_render_target_view, NULL);
			render_target_view.Reset();
#if 0
			immediate_context->Flush();
			immediate_context->ClearState();
#endif
			DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
			swap_chain->GetDesc(&swap_chain_desc);
			hr = swap_chain->ResizeBuffers(swap_chain_desc.BufferCount, framebuffer_dimensions.cx, framebuffer_dimensions.cy, swap_chain_desc.BufferDesc.Format, swap_chain_desc.Flags);
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

			Microsoft::WRL::ComPtr<ID3D11Texture2D> render_target_buffer;
			hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(render_target_buffer.GetAddressOf()));
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			D3D11_TEXTURE2D_DESC texture2d_desc;
			render_target_buffer->GetDesc(&texture2d_desc);

			hr = device->CreateRenderTargetView(render_target_buffer.Get(), NULL, render_target_view.ReleaseAndGetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}
		else
		{
			BOOL allow_tearing = FALSE;
			if (SUCCEEDED(hr))
			{
				hr = dxgi_factory6->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing, sizeof(allow_tearing));
			}
			tearing_supported = SUCCEEDED(hr) && allow_tearing;

			DXGI_SWAP_CHAIN_DESC1 swap_chain_desc1{};
			swap_chain_desc1.Width = framebuffer_dimensions.cx;
			swap_chain_desc1.Height = framebuffer_dimensions.cy;
			swap_chain_desc1.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			swap_chain_desc1.Stereo = FALSE;
			swap_chain_desc1.SampleDesc.Count = 1;
			swap_chain_desc1.SampleDesc.Quality = 0;
			swap_chain_desc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swap_chain_desc1.BufferCount = 2;
			swap_chain_desc1.Scaling = DXGI_SCALING_STRETCH;
			swap_chain_desc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swap_chain_desc1.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			swap_chain_desc1.Flags = tearing_supported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
			hr = dxgi_factory6->CreateSwapChainForHwnd(device.Get(), hwnd, &swap_chain_desc1, NULL, NULL, swap_chain.ReleaseAndGetAddressOf());
#if 0
			swap_chain_desc1.Flags |= DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
#endif
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

			hr = dxgi_factory6->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

			Microsoft::WRL::ComPtr<ID3D11Texture2D> render_target_buffer;
			hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(render_target_buffer.GetAddressOf()));
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			hr = device->CreateRenderTargetView(render_target_buffer.Get(), NULL, render_target_view.ReleaseAndGetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}

		Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_buffer{};
		D3D11_TEXTURE2D_DESC texture2d_desc{};
		texture2d_desc.Width = framebuffer_dimensions.cx;
		texture2d_desc.Height = framebuffer_dimensions.cy;
		texture2d_desc.MipLevels = 1;
		texture2d_desc.ArraySize = 1;
		texture2d_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		texture2d_desc.SampleDesc.Count = 1;
		texture2d_desc.SampleDesc.Quality = 0;
		texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
		texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texture2d_desc.CPUAccessFlags = 0;
		texture2d_desc.MiscFlags = 0;
		hr = device->CreateTexture2D(&texture2d_desc, NULL, depth_stencil_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};
		depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depth_stencil_view_desc.Texture2D.MipSlice = 0;
		hr = device->CreateDepthStencilView(depth_stencil_buffer.Get(), &depth_stencil_view_desc, depth_stencil_view.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		D3D11_VIEWPORT viewport{};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(framebuffer_dimensions.cx);
		viewport.Height = static_cast<float>(framebuffer_dimensions.cy);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		immediate_context->RSSetViewports(1, &viewport);
	}

#ifdef ENABLE_DIRECT2D
	void Graphics::create_direct2d_objects()
	{
		HRESULT hr{ S_OK };

		Microsoft::WRL::ComPtr<IDXGIDevice2> dxgi_device2;
		hr = device.As(&dxgi_device2);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		Microsoft::WRL::ComPtr<ID2D1Factory1> d2d_factory1;
		D2D1_FACTORY_OPTIONS factory_options{};
#ifdef _DEBUG
		factory_options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factory_options, d2d_factory1.GetAddressOf());

		Microsoft::WRL::ComPtr<ID2D1Device> d2d_device;
		hr = d2d_factory1->CreateDevice(dxgi_device2.Get(), d2d_device.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = d2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, d2d1_device_context.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = dxgi_device2->SetMaximumFrameLatency(1);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		Microsoft::WRL::ComPtr<IDXGISurface2> dxgi_surface2;
		hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(dxgi_surface2.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		Microsoft::WRL::ComPtr<ID2D1Bitmap1> d2d_bitmap1;
		hr = d2d1_device_context->CreateBitmapFromDxgiSurface(dxgi_surface2.Get(),
			D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)), d2d_bitmap1.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		d2d1_device_context->SetTarget(d2d_bitmap1.Get());


		Microsoft::WRL::ComPtr<IDWriteFactory> dwrite_factory;
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(dwrite_factory.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = dwrite_factory->CreateTextFormat(L"Meiryo", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 11, L"", dwrite_text_formats[0].ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		hr = dwrite_text_formats[0]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = dwrite_factory->CreateTextFormat(L"Impact", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 24, L"", dwrite_text_formats[1].ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		hr = dwrite_text_formats[1]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = d2d1_device_context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), d2d_solid_color_brushes[0].ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		hr = d2d1_device_context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue), d2d_solid_color_brushes[1].ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
#endif

	void Graphics::fullscreen_state(BOOL fullscreen)
	{
		fullscreen_mode = fullscreen;
		if (fullscreen)
		{
			GetWindowRect(hwnd, &windowed_rect);
			SetWindowLongPtrA(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME));

			RECT fullscreen_window_rect;

			HRESULT hr{ E_FAIL };
			if (swap_chain)
			{
				Microsoft::WRL::ComPtr<IDXGIOutput> dxgi_output;
				hr = swap_chain->GetContainingOutput(&dxgi_output);
				if (hr == S_OK)
				{
					DXGI_OUTPUT_DESC output_desc;
					hr = dxgi_output->GetDesc(&output_desc);
					if (hr == S_OK)
					{
						fullscreen_window_rect = output_desc.DesktopCoordinates;
					}
				}
			}
			if (hr != S_OK)
			{
				DEVMODE devmode = {};
				devmode.dmSize = sizeof(DEVMODE);
				EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);

				fullscreen_window_rect = {
					devmode.dmPosition.x,
					devmode.dmPosition.y,
					devmode.dmPosition.x + static_cast<LONG>(devmode.dmPelsWidth),
					devmode.dmPosition.y + static_cast<LONG>(devmode.dmPelsHeight)
				};
			}
			SetWindowPos(
				hwnd,
#ifdef _DEBUG
				NULL,
#else
				HWND_TOPMOST,
#endif
				fullscreen_window_rect.left,
				fullscreen_window_rect.top,
				fullscreen_window_rect.right,
				fullscreen_window_rect.bottom,
				SWP_FRAMECHANGED | SWP_NOACTIVATE);

			ShowWindow(hwnd, SW_MAXIMIZE);
		}
		else
		{
			SetWindowLongPtrA(hwnd, GWL_STYLE, windowed_style);
			SetWindowPos(
				hwnd,
				HWND_NOTOPMOST,
				windowed_rect.left,
				windowed_rect.top,
				windowed_rect.right - windowed_rect.left,
				windowed_rect.bottom - windowed_rect.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE);

			ShowWindow(hwnd, SW_NORMAL);
		}
	}

	void Graphics::on_size_changed(UINT64 width, UINT height)
	{
		HRESULT hr{ S_OK };
		if (width != framebuffer_dimensions.cx || height != framebuffer_dimensions.cy)
		{
			framebuffer_dimensions.cx = static_cast<LONG>(width);
			framebuffer_dimensions.cy = height;

			// Release all objects that hold shader resource views here.
#ifdef ENABLE_DIRECT2D
			d2d1_device_context.Reset();
#endif

			Microsoft::WRL::ComPtr<IDXGIFactory6> dxgi_factory6;
			hr = swap_chain->GetParent(IID_PPV_ARGS(dxgi_factory6.GetAddressOf()));
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			create_swap_chain(dxgi_factory6.Get());

			// Recreate all objects that hold shader resource views here.
#ifdef ENABLE_DIRECT2D
			create_direct2d_objects();
#endif
		}
	}

	void Graphics::initialize(HWND hwnd, bool fullscreen)
	{

	}

	void Graphics::render(float elapsed_time)
	{
		// ビューポートの設定
#if 0
		D3D11_VIEWPORT viewport;
		UINT num_viewports{ 1 };
		immediate_context->RSGetViewports(&num_viewports, &viewport);

		float aspect_ratio{ viewport.Width / viewport.Height };


		// 視線行列を生成
		DirectX::XMVECTOR eye{ DirectX::XMLoadFloat4(&camera_position) };
		DirectX::XMVECTOR focus{ DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f) };
		DirectX::XMVECTOR up{ DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) };
		DirectX::XMMATRIX V{ DirectX::XMMatrixLookAtLH(eye, focus, up) };

		// 射影行列を生成
		DirectX::XMMATRIX P{ DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30), aspect_ratio, 0.1f, 100.0f) };
		bit_block_transfer->blit(immediate_context.Get(), framebuffers[1]->shader_resource_views[0].GetAddressOf(), 0, 1);
#endif
#if 0
		bit_block_transfer->blit(immediate_context.Get(), framebuffers[0]->shader_resource_views[0].GetAddressOf(), 0, 1);
#endif
		UINT sync_interval{ 0 };
		swap_chain->Present(sync_interval, 0);// FrontバッファとBackバッファを入れ替える
		// ↓この下にspriteしても何も出ない
	}
}