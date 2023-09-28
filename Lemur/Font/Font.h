#pragma once
#include "../Graphics/Graphics.h"

namespace Lemur::Graphics
{
    class Font
    {
    public:
        static Font& Instance() { return *fontInstance; }
        void initialize();
        void render();
    private:
        ////TODO Font
        //ID2D1Factory* g_pD2DFactory = nullptr;
        //IDXGISurface* g_pBackBuffer = nullptr;
        //IDWriteFactory* g_pDWriteFactory = nullptr;
        //IDWriteTextFormat* g_pTextFormat = nullptr;
        //ID2D1RenderTarget* g_pRT = nullptr;
        //ID2D1SolidColorBrush* g_pSolidBrush = nullptr;

        static Font* fontInstance;
    };
}