/** @file *//********************************************************************************************************

                                                      LensFlare.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/DxObjects/LensFlare/LensFlare.h#2 $

	$NoKeywords: $

*********************************************************************************************************************/

#if !defined( DXOBJECTS_LENSFLARE_H_INCLUDED )
#define DXOBJECTS_LENSFLARE_H_INCLUDED

#pragma once

#include <windows.h>
#include <d3dx9.h>

#include "Dxx/Camera.h"

#include <vector>

namespace DxObjects
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

/// A class that draws lens flare
//
/// @ingroup	DxObjects
///

class LensFlare
{
public:

	/// Info about an element

	struct	Element
	{
		float			h;		///< Relative distance from the center (source is at 1.0, center is at 0.0)
		D3DXVECTOR2		uv0;	///< Coordinates of the upper-left corner in the texture. Range is [0,1]
		D3DXVECTOR2		uv1;	///< Coordinates of the lower-right corner in the texture. Range is [0,1]
	};

	typedef std::vector< Element >	ElementList;	///< List of elements

	/// Constructor
	LensFlare( IDirect3DDevice9 * pD3dDevice, char const * sPath, ElementList const & elements );

	/// Destructor
	virtual ~LensFlare();

	/// Draws the lens flare
	void Draw( D3DXVECTOR3 const & position, Dxx::Camera const & camera, float intensity );

private:

	IDirect3DDevice9 *			m_pD3dDevice;
	IDirect3DTexture9 *			m_pTexture;
	IDirect3DVertexBuffer9 *	m_pVB;
	ElementList					m_Elements;
};


} // namespace DxObjects


#endif // !defined( DXOBJECTS_LENSFLARE_H_INCLUDED )