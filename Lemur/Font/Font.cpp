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

		//--------------------------★追加↓--------------------------
		// テキストの描画
		WCHAR wcText1[] = L"親譲りの無鉄砲で小供の時から損ばかりしている。小学校に居る時分学校の二階から";
		WCHAR wcText2[] = L"飛び降りて一週間ほど腰を抜かした事がある。なぜそんな無闇をしたと聞く人があるか";
		WCHAR wcText3[] = L"も知れぬ。別段深い理由でもない。新築の二階から首を出していたら、同級生の一人が";
		WCHAR wcText4[] = L"冗談に、いくら威張っても、そこから飛び降りる事は出来まい。弱虫やーい。と囃した";
		WCHAR wcText5[] = L"からである。小使に負ぶさって帰って来た時、おやじが大きな眼をして二階ぐらいから";
		WCHAR wcText6[] = L"飛び降りて腰を抜かす奴があるかと云ったから、この次は抜かさずに飛んで見せますと";
		WCHAR wcText7[] = L"答えた。";

		graphics.g_pRT->BeginDraw();
		graphics.g_pRT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, graphics.g_pTextFormat, D2D1::RectF(0, 0, 800, 20), graphics.g_pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		graphics.g_pRT->DrawText(wcText2, ARRAYSIZE(wcText2) - 1, graphics.g_pTextFormat, D2D1::RectF(0, 20, 800, 40), graphics.g_pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		graphics.g_pRT->DrawText(wcText3, ARRAYSIZE(wcText3) - 1, graphics.g_pTextFormat, D2D1::RectF(0, 40, 800, 60), graphics.g_pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		graphics.g_pRT->DrawText(wcText4, ARRAYSIZE(wcText4) - 1, graphics.g_pTextFormat, D2D1::RectF(0, 60, 800, 80), graphics.g_pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		graphics.g_pRT->DrawText(wcText5, ARRAYSIZE(wcText5) - 1, graphics.g_pTextFormat, D2D1::RectF(0, 80, 800, 100), graphics.g_pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		graphics.g_pRT->DrawText(wcText6, ARRAYSIZE(wcText6) - 1, graphics.g_pTextFormat, D2D1::RectF(0, 100, 800, 120), graphics.g_pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		graphics.g_pRT->DrawText(wcText7, ARRAYSIZE(wcText7) - 1, graphics.g_pTextFormat, D2D1::RectF(0, 120, 800, 140), graphics.g_pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		graphics.g_pRT->EndDraw();
		//--------------------------★追加↑--------------------------
	}
};