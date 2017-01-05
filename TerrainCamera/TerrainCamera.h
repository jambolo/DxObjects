/** @file *//********************************************************************************************************

                                                   TerrainCamera.h

						                    Copyright 2003, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/DxObjects/TerrainCamera/TerrainCamera.h#3 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once


#include "Dxx/Camera.h"

namespace DxObjects
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

//! A camera for use in situations where the "up" direction is fixed.
//!
//! @ingroup	DxObjects
//!
//! The camera is controlled by Euler angles; however, yaw is always around the world's Y axis. In addition, the
//! range of the yaw is -pi/2 < yaw < pi/2, and the range of the roll is -pi/2 < roll < pi/2.

class TerrainCamera : public Dxx::Camera
{
public:

	//! Constructor
	TerrainCamera( IDirect3DDevice9 * pDevice,
				   float angleOfView,
				   float nearDistance,
				   float farDistance,
				   float aspectRatio,
				   D3DXVECTOR3 const & position		= Dxx::Vector3Origin(),
				   float yaw						= 0.f,
				   float pitch						= 0.f,
				   float roll						= 0.f );

	//! Destructor
	virtual ~TerrainCamera();

	//! @name	Overrides Camera
	//@{
	//	void Reshape( int w, int h, float x = 0.f, float y = 0.f );
	//	void Look() const;
	//	void SetFrame( Frame const & frame );
	//	Frame GetFrame() const;
	//	void SetPosition( D3DXVECTOR3 const & position );
	//	D3DXVECTOR3 GetPosition() const;
	//	void SetOrientation( D3DXQUATERNION const & orientation );
	//	D3DXQUATERNION GetOrientation() const;
	//	void LookAt( D3DXVECTOR3 const & to, D3DXVECTOR3 const & from, D3DXVECTOR3 const & up );
	//	void SetNearDistance( float nearDistance );
	//	float GetNearDistance() const;
	//	void SetFarDistance( float farDistance );
	//	float GetFarDistance() const;
	//	void SetAngleOfView( float angle );
	//	float GetAngleOfView() const;
	//	void Turn( D3DXQUATERNION const & rotation );
	//	void Turn( float angle, D3DXVECTOR3 const & axis );
	//	void Move( D3DXVECTOR3 const & distance );
	//	D3DXVECTOR3 GetDirection() const;
	//	D3DXVECTOR3 GetUp() const;
	//	D3DXVECTOR3 GetRight() const;
	//	D3DXMATRIX GetViewMatrix() const;
	//	D3DXMATRIX GetProjectionMatrix() const;
	//	D3DXMATRIX GetViewProjectionMatrix() const;
	//@}

	//! Turns the camera right or left
	void Yaw( float angle );

	//! Pitches the camera up or down
	void Pitch( float angle );

	//! Rolls the camera CW or CCW
	void Roll( float angle );
};


} // namespace DxObjects
