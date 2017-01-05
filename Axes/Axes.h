/** @file *//********************************************************************************************************

                                                        Axes.h

						                    Copyright 2006, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/DxObjects/Axes/Axes.h#1 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once


#if defined( _DEBUG )
	#define D3D_DEBUG_INFO
#endif
#include <d3dx9math.h>
#include <Dxx/D3d.h>
#include <boost/noncopyable.hpp>

struct IDirect3DDevice9;

namespace DxObjects
{

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

class Axes : boost::noncopyable
{
public:

	//! Constructor
	explicit Axes( IDirect3DDevice9 * pD3dDevice, float size = 1.0f );

	//! Destructor
	virtual ~Axes();

	void Draw();

private:

	IDirect3DDevice9 *	m_pD3dDevice;
	float				m_size;
};


} // namespace DxObjects