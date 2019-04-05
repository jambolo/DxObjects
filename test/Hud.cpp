#include "Hud.h"

#include "DxUtility/dxutil.h"
#include "Time/FrameRateCalculator.h"
#include "Wx/Wx.h"

#include <cstdio>
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>

Hud::Hud(IDirect3DDevice9 * pD3dDevice)
    : m_pD3dDevice(pD3dDevice)
{
    pD3dDevice->AddRef();

    InitializeFonts(pD3dDevice);
}

Hud::~Hud()
{
    ShutDownFonts();

    SAFE_RELEASE(m_pD3dDevice)
}

void Hud::Update(__int64 t)
{
    m_FrameRateCalculator.Update(t);
}

void Hud::Draw()
{
    char buffer [256];

    sprintf(buffer, "Frame rate: %6.2f fps (%6.2f fps average)",
            m_FrameRateCalculator.GetFrameRate(),
            m_FrameRateCalculator.GetAverageFrameRate());

    RECT rect = { 0, 0, 320, 40 };
    m_pFont->DrawText(buffer, -1, &rect, 0, D3DCOLOR_XRGB(255, 255, 0));
}

void Hud::InitializeFonts(IDirect3DDevice9 * pD3dDevice)
{
    OutputDebugString("***InitializeFonts\n");

    HRESULT hr;

    static LOGFONT lf =
    {
        16, 0,
        0, 0,
        FW_BOLD, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE,
        "Courier New"
    };

    hr = D3DXCreateFontIndirect(pD3dDevice, &lf, &m_pFont);
    assert_succeeded(hr);
}

void Hud::ShutDownFonts()
{
    OutputDebugString("***ShutDownFonts\n");

    SAFE_RELEASE(m_pFont);
}
