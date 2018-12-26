/** @file *//********************************************************************************************************

                                                  TerrainCamera.cpp

						                    Copyright 2003, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/DxObjects/TerrainCamera/TerrainCamera.cpp#4 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "DxObjects/TerrainCamera.h"

#include "Math/Quaternion.h"
#include "Math/Constants.h"
#include "Misc/max.h"


namespace DxObjects
{

//! @param	angleOfView		Angle of view  (in degrees). Valid range is (0,90).
//! @param	nearDistance	Distance to the near plane
//! @param	farDistance		Distance to the far plane
//! @param	position		Initial position
//! @param	yaw				Initial yaw (in degrees)
//! @param	pitch			Initial pitch (in degrees)
//! @param	roll			Initial roll (in degrees)
//!
//! @note	The unrotated position of the camera is: facing along X, Z is up, Y is right. The initial orientation
//!			angles are applied in this order: yaw, pitch, roll.

TerrainCamera::TerrainCamera( IDirect3DDevice9 *	pDevice,
							  float					angleOfView,
							  float					nearDistance,
							  float					farDistance,
							  float					aspectRatio,
							  D3DXVECTOR3 const &	position		/*= Dxx::Vector3Origin()*/,
							  float					yaw				/*= 0.f*/,
							  float					pitch			/*= 0.f*/,
							  float					roll			/*= 0.f*/ )
	: Dxx::Camera( pDevice,
				   angleOfView,
				   nearDistance, farDistance,
				   aspectRatio,
				   position,
				   D3DXQUATERNION( (float)Math::SQRT_OF_3_OVER_3 * 0.866025403f,
								   (float)Math::SQRT_OF_3_OVER_3 * 0.866025403f,
								   (float)Math::SQRT_OF_3_OVER_3 * 0.866025403f,
								   0.5f ) )
{
	Yaw( yaw );
	Pitch( pitch );
	Roll( roll );
}

//! @param	angle	Angle of rotation in degrees.
//!
//! @note	Yaw for this camera is a rotation around the @b world Z axis.

void TerrainCamera::Yaw( float angle )
{
	// We need the Z axis from the inverse of the camera's orientation, which is conveniently located in the
	// camera's view matrix.

	D3DXVECTOR3	worldYAxis( &GetViewMatrix().m[2][0] );

	Turn( angle, worldYAxis );
}

//!
//! @param	angle	Angle of rotation in degrees.

void TerrainCamera::Pitch( float angle )
{
	Turn( angle, Dxx::Vector3XAxis() );
}

//!
//! @param	angle	Angle of rotation in degrees.

void TerrainCamera::Roll( float angle )
{
	Turn( angle, Dxx::Vector3ZAxis() );
}


} // namespace DxObjects