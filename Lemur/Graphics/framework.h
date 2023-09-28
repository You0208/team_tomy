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

	int run()
	{
		MSG msg{};

		if (!initialize())
		{
			return 0;
		}

		// ImGui‰Šú‰»(DirectX11‚Ì‰Šú‰»‚Ì‰º‚É’u‚­‚±‚Æ)
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
				tictoc.tick();
				calculate_frame_stats();
				update(tictoc.time_interval());
				render(tictoc.time_interval());
			}
		}

		// ImGuiI—¹‰»
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
		// ImGui(æ“ª‚É’u‚­)
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
			tictoc.stop();
			break;
		case WM_EXITSIZEMOVE:
			tictoc.start();
			break;
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


	high_resolution_timer tictoc;
	uint32_t frames{ 0 };
	float elapsed_time{ 0.0f };
	void calculate_frame_stats()
	{
		if (++frames, (tictoc.time_stamp() - elapsed_time) >= 1.0f)
		{
			float fps = static_cast<float>(frames);
			std::wostringstream outs;
			outs.precision(6);
			outs << APPLICATION_NAME << L" : FPS : " << fps << L" / " << L"Frame Time : " << 1000.0f / fps << L" (ms)";
			SetWindowTextW(hwnd, outs.str().c_str());

			frames = 0;
			elapsed_time += 1.0f;
		}
	}
};

