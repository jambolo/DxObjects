/** @file *//********************************************************************************************************

                                                       SkyBox.h

                                            Copyright 2003, John J. Bolton
    --------------------------------------------------------------------------------------------------------------

    $Header: //depot/DxObjects/SkyBox/SkyBox.h#6 $

    $NoKeywords: $

********************************************************************************************************************/

#pragma once

#include "Dxx/Camera.h"
#if defined(_DEBUG)
#define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>

namespace DxObjects
{
//! A D3D-based skybox.
//
//!
//! @ingroup	DxObjects

class SkyBox
{
public:

    //! Skybox faces
    enum Faces
    {
        FACE_FRONT = 0x01,          //!< +Z
        FACE_BACK  = 0x02,          //!< -Z
        FACE_RIGHT = 0x04,          //!< +X
        FACE_LEFT  = 0x08,          //!< -X
        FACE_UP    = 0x10,          //!< +Y
        FACE_DOWN  = 0x20,          //!< -Y

        FACE_ALL_FACES  = 0x3f,
        FACE_TOP_FACES  = 0x1f,     //!< All faces except FACE_DOWN
        FACE_SIDE_FACES = 0x0f,     //!< All faces except FACE_UP and FACE_DOWN

        NUM_FACES = 6,
    };

    //! Constructor
    SkyBox(IDirect3DDevice9 * pD3dDevice,
           char const *       sFileName,
           char const *       sExt     = ".tga",
           unsigned           faceMask = FACE_ALL_FACES);

    //! Destructor
    ~SkyBox();

    //! Draws the skybox
    void Draw(Dxx::Camera const & camera, bool bTestZ = false);

    //! Draws the skybox
    void Draw(D3DXVECTOR3 const & vp, float r, bool bTestZ = false);

private:

    // Vertex buffer info

    struct VBEntry
    {
        D3DVECTOR position;
        float u, v;
    };

    enum
    {
        FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0)
    };

    IDirect3DDevice9 * m_pD3dDevice;
    IDirect3DTexture9 * m_apTextures[NUM_FACES];
    IDirect3DVertexBuffer9 * m_pVB;
};
} // namespace DxObjects
