#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>

#include "misc.h"
#include "sprite.h"
#include "sprite_batch.h"
#include "high_resolution_timer.h"
#include "geometric_primitive.h"
#include "./Lemur/Model/Model.h"
#include "framebuffer.h"
#include "fullscreen_quad.h"
#include "..\Lemur.h"
#include "Graphics.h"
#include "..\Input/Input.h"


#include "../Scene/BaseScene.h"
#include "../Scene/DemoScene.h"

#include <d3d11.h>
#include <wrl.h>

#include "./ImGuiCtrl.h"

CONST LPCWSTR APPLICATION_NAME{ L"X3DGP" };

class framework
{
private:
	Lemur::Graphics::Graphics graphics;
	Input input;

public:
	CONST HWND hwnd;



	float timer{ 0.0f };
	framework(HWND hwnd);
	~framework();

	framework(const framework&) = delete;
	framework& operator=(const framework&) = delete;
	framework(framework&&) noexcept = delete;
	framework& operator=(framework&&) noexcept = delete;

	void on_size_changed(UINT64 width, UINT height);

	int run()
	{
		MSG msg{};

		if (!initialize())
		{
			return 0;
		}

		// ImGui初期化(DirectX11の初期化の下に置くこと)
		IMGUI_CTRL_INITIALIZE(hwnd, graphics.GetDevice(), graphics.GetDeviceContext());

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				high_resolution_timer::Instance().tick();
				calculate_frame_stats();
				update(high_resolution_timer::Instance().time_interval());
				render(high_resolution_timer::Instance().time_interval());
			}
		}

		// ImGui終了化
		IMGUI_CTRL_UNINITIALIZE();


#if 0
		BOOL fullscreen = 0;
		swap_chain->GetFullscreenState(&fullscreen, 0);
		if (fullscreen)
		{
			swap_chain->SetFullscreenState(FALSE, 0);
		}
#endif

		return uninitialize() ? static_cast<int>(msg.wParam) : 0;
	}

	LRESULT CALLBACK handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		// ImGui(先頭に置く)
		IMGUI_CTRL_WND_PRC_HANDLER(hwnd, msg, wparam, lparam);

		switch (msg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps{};
			BeginPaint(hwnd, &ps);

			EndPaint(hwnd, &ps);
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CREATE:
			break;
		case WM_KEYDOWN:
			if (wparam == VK_ESCAPE)
			{
				PostMessage(hwnd, WM_CLOSE, 0, 0);
			}
			break;
		case WM_ENTERSIZEMOVE:
			high_resolution_timer::Instance().stop();
			break;
		case WM_EXITSIZEMOVE:
			high_resolution_timer::Instance().start();
			break;
		case WM_SIZE:
		{
#if 1
			RECT client_rect{};
			GetClientRect(hwnd, &client_rect);
			on_size_changed(static_cast<UINT64>(client_rect.right - client_rect.left), client_rect.bottom - client_rect.top);
#endif
			break;
		}
		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

private:
	bool initialize();
	void update(float elapsed_time/*Elapsed seconds from last frame*/);
	void render(float elapsed_time/*Elapsed seconds from last frame*/);
	bool uninitialize();

private:
	//high_resolution_timer tictoc;
	uint32_t frames{ 0 };
	float elapsed_time{ 0.0f };
	void calculate_frame_stats()
	{
		if (++frames, (high_resolution_timer::Instance().time_stamp() - elapsed_time) >= 1.0f)
		{
			float fps = static_cast<float>(frames);
			std::wostringstream outs;
			outs.precision(6);
			outs <<  L" ARACHNO GAMBLE ";
			SetWindowTextW(hwnd, outs.str().c_str());

			frames = 0;
			elapsed_time += 1.0f;
		}
	}
};

