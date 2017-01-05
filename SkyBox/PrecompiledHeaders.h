/********************************************************************************************************************

                                                 PrecompiledHeaders.h

						                    Copyright 2003, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/DxObjects/SkyBox/PrecompiledHeaders.h#3 $

	$NoKeywords: $

 ********************************************************************************************************************/

#define STRICT
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

/********************************************************************************************************************/
/*																													*/
/* Project header files																								*/
/*																													*/
/********************************************************************************************************************/

/********************************************************************************************************************/
/*																													*/
/* Library header files																								*/
/*																													*/
/********************************************************************************************************************/

/********************************************************************************************************************/
/*																													*/
/* Windows header files																								*/
/*																													*/
/********************************************************************************************************************/

#include <windows.h>
#if defined( _DEBUG )
	#define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>

/********************************************************************************************************************/
/*																													*/
/* Standard Library header files																					*/
/*																													*/
/********************************************************************************************************************/
