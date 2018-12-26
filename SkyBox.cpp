/** @file *//********************************************************************************************************

                                                      SkyBox.cpp

						                    Copyright 2003, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/DxObjects/SkyBox/SkyBox.cpp#6 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "DxObjects/SkyBox.h"

#include "Dxx/Dxx.h"
#include "Misc/Exceptions.h"
#include "Misc/SafeStr.h"
#include "Wx/Wx.h"


namespace DxObjects
{


//! @param	pD3dDevice	Device the skybox will be drawn on
//! @param	sFilename	Base file name of the files containing the skybox textures.
//! @param	sExt		Filename extension (including the '.')
//! @param	faceMask	Which faces of the skybox are to be drawn. This value is set by ORing the appropriate Faces
//!						values together. Valid faces are:
//!							- @c FACE_FRONT (+Z)
//!							- @c FACE_BACK (-Z)
//!							- @c FACE_RIGHT (+X)
//!							- @c FACE_LEFT (-X)
//!							- @c FACE_UP (+Y)
//!							- @c FACE_DOWN (-Y)
//!							- @c FACE_ALL_FACES		
//!							- @c FACE_TOP_FACES (all except @c FACE_DOWN)
//!							- @c FACE_SIDE_FACES (all except @c FACE_UP and @c FACE_DOWN)
//!
//! @exception	ConstructorFailedException		The textures cannot be loaded.
//!
//! @note	The actual files names for each face are created by appending one of "_+Z", "_-Z", "_+X", "_-X", "_+Y",
//!			or "_-Y", followed by the extension specified by @a sExt to the base name specified by @a sFilename.
//!	@note	Only textures for faces specified by @a facemask are loaded.

SkyBox::SkyBox( IDirect3DDevice9 * pD3dDevice, char const * sFileName, char const * sExt/* = ".tga"*/, unsigned faceMask/* = FACE_ALL_FACES*/ )
	: m_pD3dDevice( pD3dDevice )
{
	HRESULT		hr;

	pD3dDevice->AddRef();

	for ( int face = 0; face < NUM_FACES; face++ )
	{
		if ( ( faceMask & ( 1 << face ) ) != 0 )
		{
			static char const	aSuffixes[ NUM_FACES ][4] =
								{
									"_+Z",
									"_-Z",
									"_+X",
									"_-X",
									"_+Y",
									"_-Y"
								};

			// Create the full name of the file

			char path[ _MAX_PATH ];

			SafeStrncpy( path, sFileName, sizeof( path ) );
			SafeStrncat( path, aSuffixes[ face ], sizeof( path ) );
			SafeStrncat( path, sExt, sizeof( path ) );

			hr = D3DXCreateTextureFromFileEx( pD3dDevice, path, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN,
											  D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,
											  &m_apTextures[ face ] );
			if ( FAILED( hr ) )
			{
				throw ConstructorFailedException( "Unable to load skybox texture" );
			}
		}
		else
		{
			m_apTextures[ face ] = 0;
		}
	}

	// Create the vertex buffer

	static VBEntry const aVertices[ NUM_FACES ][ 4 ] =
	{
		// FACE_FRONT (+Z)
		{
			{ { -1.0f,  1.0f,  1.0f }, 0.0f, 0.0f },
			{ {  1.0f,  1.0f,  1.0f }, 1.0f, 0.0f },
			{ { -1.0f, -1.0f,  1.0f }, 0.0f, 1.0f },
			{ {  1.0f, -1.0f,  1.0f }, 1.0f, 1.0f }
		},
		// FACE_BACK (-Z)
		{
			{ { -1.0f, -1.0f, -1.0f }, 0.0f, 0.0f },
			{ {  1.0f, -1.0f, -1.0f }, 1.0f, 0.0f },
			{ { -1.0f,  1.0f, -1.0f }, 0.0f, 1.0f },
			{ {  1.0f,  1.0f, -1.0f }, 1.0f, 1.0f }
		},
		// FACE_RIGHT (+X)
		{
			{ {  1.0f, -1.0f,  1.0f }, 0.0f, 0.0f },
			{ {  1.0f,  1.0f,  1.0f }, 1.0f, 0.0f },
			{ {  1.0f, -1.0f, -1.0f }, 0.0f, 1.0f },
			{ {  1.0f,  1.0f, -1.0f }, 1.0f, 1.0f }
		},
		// FACE_LEFT (-X)
		{
			{ { -1.0f,  1.0f,  1.0f }, 0.0f, 0.0f },
			{ { -1.0f, -1.0f,  1.0f }, 1.0f, 0.0f },
			{ { -1.0f,  1.0f, -1.0f }, 0.0f, 1.0f },
			{ { -1.0f, -1.0f, -1.0f }, 1.0f, 1.0f }
		},
		// FACE_UP (+Y)
		{
			{ {  1.0f,  1.0f,  1.0f }, 0.0f, 0.0f },
			{ { -1.0f,  1.0f,  1.0f }, 1.0f, 0.0f },
			{ {  1.0f,  1.0f, -1.0f }, 0.0f, 1.0f },
			{ { -1.0f,  1.0f, -1.0f }, 1.0f, 1.0f }
		},
	// FACE_DOWN (-Y)
		{
			{ { -1.0f, -1.0f,  1.0f }, 0.0f, 0.0f },
			{ {  1.0f, -1.0f,  1.0f }, 1.0f, 0.0f },
			{ { -1.0f, -1.0f, -1.0f }, 0.0f, 1.0f },
			{ {  1.0f, -1.0f, -1.0f }, 1.0f, 1.0f }
		},
	};

	hr = Dxx::CreateStaticVertexBuffer( m_pD3dDevice, aVertices, sizeof( aVertices ), FVF, &m_pVB );
}

SkyBox::~SkyBox()
{
	Wx::SafeRelease( m_pVB );
	for ( int face = 0; face < NUM_FACES; face++ )
	{
		Wx::SafeRelease( m_apTextures[ face ] );
	}

	Wx::SafeRelease( m_pD3dDevice );
}

//! @param	camera	The camera for the scene
//! @param	bTestZ	If @c true, then enable depth-testing when drawing
//!
//! @note	The following states are set:
//!				- SetTransform( D3DTS_WORLD, ... )
//!				- SetRenderState( D3DRS_ZENABLE, ... )
//!				- SetRenderState( D3DRS_ZWRITEENABLE, FALSE )
//!				- SetRenderState( D3DRS_LIGHTING, FALSE )
//!				- SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
//!				- SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
//!				- SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
//!				- SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
//!				- SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
//!				- SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//!				- SetTexture( 0, ... );
//!
//! @note	Depth-testing is necessary only if you do not render the skybox first. Obviously, if depth-testing is
//!			enabled, the depth-buffer must be initialized beforehand.


void SkyBox::Draw( Dxx::Camera const & camera, bool bTestZ/* = false*/ )
{
	// Draw the skybox using this camera. Center the sides between the near and far clip planes (to make sure they
	// aren't clipped.

	Draw( camera.GetPosition(),
		  ( camera.GetNearDistance() + camera.GetFarDistance() ) / ( 1.f + float( Math::SQRT_OF_3 ) ),
		  bTestZ );
}

//! @param	vp	Location of the view point (the skybox is centered at this point)
//! @param	r	Distance from the center of the skybox to a side.
//! @param	bTestZ	If @c true, then enable depth-testing when rendering
//!
//! @note	The following states are set:
//!				- SetTransform( D3DTS_WORLD, ... )
//!				- SetRenderState( D3DRS_ZENABLE, ... )
//!				- SetRenderState( D3DRS_ZWRITEENABLE, FALSE )
//!				- SetRenderState( D3DRS_LIGHTING, FALSE )
//!				- SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
//!				- SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
//!				- SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
//!				- SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
//!				- SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
//!				- SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//!				- SetTexture( 0, ... );
//!				- SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
//!
//! @note	To ensure that the sides and corners of the skybox are not clipped by the near and far planes, use this
//!			value for @a r: <tt>r = ( n + f ) / ( 1 + sqrt(3) )</tt>. The skybox will be not be clipped as long as
//!			<tt>f >= n * sqrt(3)</tt>
//!
//! @note	Depth-testing is necessary only if you do not render the skybox first. Obviously, if depth-testing is
//!			enabled, the depth-buffer must be initialized beforehand.

void SkyBox::Draw( D3DXVECTOR3 const & vp, float r, bool bTestZ/* = false*/ )
{
	HRESULT		hr;

	// Set Z-test if desired, otherwise disable it

	if ( bTestZ )
	{
		hr = m_pD3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	}
	else
	{
		hr = m_pD3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	}
	assert_succeeded( hr );

	// Turn off Z-write

	hr = m_pD3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	assert_succeeded( hr );

	// Turn off lighting

	hr = m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	assert_succeeded( hr );

	// Build the model-world transform. Note: The formula here is W = R * S * T, but the correct formula is
	// W = S * R * T. The order was switched because it simplifies the code. It works because S = I * r,
	// so R * S == S * R.

	D3DXMATRIX	translate;
	D3DXMATRIX	scale;
	D3DXMATRIX	world;

	D3DXMatrixTranslation( &translate, vp.x,  vp.y, vp.z );
	D3DXMatrixScaling( &scale, r, r, r );
	D3DXMatrixMultiply( &world, &scale, &translate );
	hr = m_pD3dDevice->SetTransform( D3DTS_WORLD, &world );
	assert_succeeded( hr );

	hr = m_pD3dDevice->SetFVF( FVF );
	assert_succeeded( hr );

	hr = m_pD3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	assert_succeeded( hr );

	hr = m_pD3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	assert_succeeded( hr );

	hr = m_pD3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	assert_succeeded( hr );

	hr = m_pD3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	assert_succeeded( hr );

	hr = m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	assert_succeeded( hr );

	hr = m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	assert_succeeded( hr );

	hr = m_pD3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
	assert_succeeded( hr );

	hr = m_pD3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof( VBEntry ) );
	assert_succeeded( hr );

	for ( int face = 0; face < NUM_FACES; face++ )
	{
		if ( m_apTextures[ face ] )
		{
			m_pD3dDevice->SetTexture( 0, m_apTextures[ face ] );

			m_pD3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, face * 4, 2 );
		}
	}
}


} // namespace DxObjects
