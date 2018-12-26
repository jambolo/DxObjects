/** @file *//********************************************************************************************************

                                                       Axes.cpp

                                            Copyright 2005, John J. Bolton
    --------------------------------------------------------------------------------------------------------------

    $Header: //depot/DxObjects/Axes/Axes.cpp#1 $

    $NoKeywords: $

********************************************************************************************************************/

#include "DxObjects/Axes.h"

#include <Misc/Assert.h>
#include <Misc/Etc.h>
#include <Wx/Wx.h>

namespace
{
    struct VBEntry
    {
        D3DVECTOR position;
        D3DCOLOR diffuse;
    };

    DWORD const FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
} // anonymous namespace

namespace DxObjects
{
//! @param	pD3dDevice		D3D device
//! @param	size			size of the axes

Axes::Axes(IDirect3DDevice9 * pD3dDevice, float size /* = 1.0f*/)
    : m_pD3dDevice(pD3dDevice)
    , m_size(size)
{
    m_pD3dDevice->AddRef();
}

Axes::~Axes(void)
{
    Wx::SafeRelease(m_pD3dDevice);
}

void Axes::Draw()
{
    static const VBEntry aVertexes[] =
    {
        {{ 0.0f, 0.0f, 0.0f }, 0x00ff0000 }, {{ 1.0f, 0.0f, 0.0f }, 0x00ff0000 },
        {{ 0.0f, 0.0f, 0.0f }, 0x0000ff00 }, {{ 0.0f, 1.0f, 0.0f }, 0x0000ff00 },
        {{ 0.0f, 0.0f, 0.0f }, 0x000000ff }, {{ 0.0f, 0.0f, 1.0f }, 0x000000ff }
    };

    HRESULT hr;

    // Turn off lighting and textures

    hr = m_pD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    assert_succeeded(hr);

    m_pD3dDevice->SetTexture(0, 0);

    // No rotation, no translation, no scaling, nothing.

    D3DXMATRIX world;

    D3DXMatrixIdentity(&world);
    hr = m_pD3dDevice->SetTransform(D3DTS_WORLD, &world);
    assert_succeeded(hr);

    hr = m_pD3dDevice->SetFVF(FVF);
    assert_succeeded(hr);

//	hr = m_pD3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof( VBEntry ) );
//	assert_succeeded( hr );

    m_pD3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, (UINT)elementsof(aVertexes) / 2, aVertexes, sizeof(aVertexes[0]));
}
} // namespace DxObjects
