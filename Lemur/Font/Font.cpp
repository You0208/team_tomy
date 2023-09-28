#include "Font.h"

namespace Lemur::Graphics
{
	Font* Font::fontInstance = nullptr;

	void Font::initialize()
	{
		HRESULT hr{ S_OK };

		//g_pD2DFactory = graphics.Getg_pD2DF();
		//g_pBackBuffer =graphics.Getg_pBackBuffer();
		//g_pDWriteFactory =graphics.Getg_pDWriteFactory();
		//g_pTextFormat =graphics.Getg_pTextFormat();
		//g_pRT =graphics.Getg_pRT();
		//g_pSolidBrush =graphics.Getg_pSolidBrush();

	}
	void Font::render()
	{
		Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();

		//--------------------------���ǉ���--------------------------
		// �e�L�X�g�̕`��
		WCHAR wcText1[] = L"�e����̖��S�C�ŏ����̎����瑹�΂��肵�Ă���B���w�Z�ɋ��鎞���w�Z�̓�K����";
		WCHAR wcText2[] = L"��э~��Ĉ�T�ԂقǍ��𔲂�������������B�Ȃ�����Ȗ��ł������ƕ����l�����邩";
		WCHAR wcText3[] = L"���m��ʁB�ʒi�[�����R�ł��Ȃ��B�V�z�̓�K�������o���Ă�����A�������̈�l��";
		WCHAR wcText4[] = L"��k�ɁA������В����Ă��A���������э~��鎖�͏o���܂��B�㒎��[���B�ƚ�����";
		WCHAR wcText5[] = L"����ł���B���g�ɕ��Ԃ����ċA���ė������A���₶���傫�Ȋ�����ē�K���炢����";
		WCHAR wcText6[] = L"��э~��č��𔲂����z�����邩�Ɖ]��������A���̎��͔��������ɔ��Ō����܂���";
		WCHAR wcText7[] = L"�������B";

		graphics.g_pRT->BeginDraw();
		graphics.g_pRT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, graphics.g_pTextFormat, D2D1::RectF(0, 0, 800, 20), graphics.g_pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		graphics.g_pRT->DrawText(wcText2, ARRAYSIZE(wcText2) - 1, graphics.g_pTextFormat, D2D1::RectF(0, 20, 800, 40), graphics.g_pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		graphics.g_pRT->DrawText(wcText3, ARRAYSIZE(wcText3) - 1, graphics.g_pTextFormat, D2D1::RectF(0, 40, 800, 60), graphics.g_pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		graphics.g_pRT->DrawText(wcText4, ARRAYSIZE(wcText4) - 1, graphics.g_pTextFormat, D2D1::RectF(0, 60, 800, 80), graphics.g_pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		graphics.g_pRT->DrawText(wcText5, ARRAYSIZE(wcText5) - 1, graphics.g_pTextFormat, D2D1::RectF(0, 80, 800, 100), graphics.g_pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		graphics.g_pRT->DrawText(wcText6, ARRAYSIZE(wcText6) - 1, graphics.g_pTextFormat, D2D1::RectF(0, 100, 800, 120), graphics.g_pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		graphics.g_pRT->DrawText(wcText7, ARRAYSIZE(wcText7) - 1, graphics.g_pTextFormat, D2D1::RectF(0, 120, 800, 140), graphics.g_pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		graphics.g_pRT->EndDraw();
		//--------------------------���ǉ���--------------------------
	}
};