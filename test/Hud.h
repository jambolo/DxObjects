#if !defined(HUD_H_INCLUDED)
#define HUD_H_INCLUDED

#pragma once

#include "Misc/FrameRateCalculator.h"

struct IDirect3DDevice9;
struct ID3DXFont;

class Hud
{
public:

    Hud(IDirect3DDevice9 * pD3dDevice);
    ~Hud();

    void Update(__int64 t);

    void Draw();

private:

    void InitializeFonts(IDirect3DDevice9 * pD3dDevice);
    void ShutDownFonts();

    IDirect3DDevice9 * m_pD3dDevice;
    FrameRateCalculator m_FrameRateCalculator;
    ID3DXFont * m_pFont;
};

#endif // !defined( HUD_H_INCLUDED )
