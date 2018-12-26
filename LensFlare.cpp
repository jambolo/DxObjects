/** @file *//********************************************************************************************************

                                                    LensFlare.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/DxObjects/LensFlare/LensFlare.cpp#2 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "DxObjects/LensFlare.h"

#include <windows.h>
#include <d3dx9.h>

#include "DxUtility/dxutil.h"
#include "Misc/Exceptions.h"
#include "Dxx/Camera.h"

#include <vector>


namespace DxObjects
{

/// @param	pD3dDevice	Device the skybox will be drawn on
/// @param	sPath		Location of the texture
/// @param	elements	Information about each element
///
/// @warning	A ConstructorFailedException is thrown if the textures cannot be loaded.

LensFlare::LensFlare( IDirect3DDevice9 * pD3dDevice, char const * sPath, ElementList const & elements )
	: m_pD3dDevice( pD3dDevice ), m_Elements( elements )
{
	HRESULT		hr;

	pD3dDevice->AddRef();

	hr = D3DXCreateTextureFromFileEx( pD3dDevice, sPath, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN,
									  D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,
									  &m_pTexture );
	if ( FAILED( hr ) )
	{
		throw ConstructorFailedException( "Unable to load skybox texture" );
	}
}

LensFlare::~LensFlare()
{
	SAFE_RELEASE( m_pVB );
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pD3dDevice );
}

/// This function draws the lens flare if the source is in the view frustum. The @a intensity parameter is used to
/// vary the amount of flare based on the intensity and/or occlusion of the source. The @a position and @a camera
/// parameters are used to determine if the source is in view.
///
/// @param	position	Position of the lens flare source
/// @param	camera		Camera used to draw the scene
/// @param	intensity	Relative intensity of the lens flare. 1 = full intensity, 0 = invisible

void LensFlare::Draw( D3DXVECTOR3 const & position, Dxx::Camera const & camera, float intensity )
{
	// If the intensity is 0, then there is nothing to draw.

	if ( intensity <= 0.0f )
	{
		return;
	}

	D3DXMATRIX const	viewMatrix = camera.GetViewProjectionMatrix();
}


} // namespace DxObjects
