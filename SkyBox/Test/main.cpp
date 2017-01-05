/** @file *//********************************************************************************************************

                                                       main.cpp

						                    Copyright 2003, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/DxObjects/SkyBox/Test/main.cpp#3 $

	$NoKeywords: $

 ********************************************************************************************************************/


#define	STRICT
#define	NOMINMAX
#define	WIN32_LEAN_AND_MEAN

#include "../SkyBox.h"
#include "Hud.h"

#include "DxUtility/DxUtil.h"
#include "Dxx/Dxx.h"
#include "Math/Constants.h"
#include "Time/Clock.h"
#include "Wx/Wx.h"

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <cstdlib>

static LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
static bool Update( HWND hWnd );
static void Display();
static void Reshape( HWND hWnd, int w, int h );
static void InitializeRendering( HWND hWnd );
static void ShutDownRendering();
static void InitializeApp();
static void ShutDownApp();
static void DrawTeapot();
static void DrawSphere();

static LPD3DXMESH m_pSphereMesh;          // Representation of point light

static char					s_sAppName[]	= "SkyBox";
static char					s_sTitleBar[]	= "SkyBox";

static Dxx::Camera *		s_pCamera;
static float				s_CameraSpeed				= 2.f;

static IDirect3D9 *			s_pD3d;
static IDirect3DDevice9 *	s_pD3dDevice;
static D3DCAPS9				s_Caps;

static Hud *				s_pHud;
static DxObjects::SkyBox *	s_pSkyBox;

static Dxx::DirectionalLight *	s_pLight;

static ID3DXMesh *			s_pTeapot;
static ID3DXMesh *			s_pSphere;
static D3DMATERIAL9			s_TeapotMaterial	=
							{
								{ 1.0f, 1.0f, 1.0f, 1.0f },
								{ 1.0f, 1.0f, 1.0f, 1.0f },
								{ 1.0f, 1.0f, 1.0f, 1.0f },
								{ 0.0f, 0.0f, 0.0f, 0.0f },
								0.0f
							};
static D3DMATERIAL9			s_SphereMaterial	=
							{
								{ 0.0f, 0.0f, 0.0f, 0.0f },
								{ 0.0f, 0.0f, 0.0f, 0.0f },
								{ 0.0f, 0.0f, 0.0f, 0.0f },
								{ 0.0f, 0.0f, 0.0f, 1.0f },
								0.0f
							};


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow )
{
	if ( Wx::RegisterWindowClass(	CS_OWNDC,
									( WNDPROC )WindowProc,
									hInstance,
									s_sAppName,
									LoadCursor( NULL, IDC_ARROW )
								) == NULL )
	{
		MessageBox( NULL, "Wx::RegisterWindowClass() failed.", "Error", MB_OK );
		exit( 1 );
	}

	OutputDebugString( "***CreateWindowEx\n" );
	HWND hWnd = CreateWindowEx( 0,
								s_sAppName,
								s_sTitleBar,
								WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
								0, 0, 800, 600,
								NULL,
								NULL,
								hInstance,
								NULL );

	if ( hWnd == NULL )
	{
		MessageBox( NULL, "CreateWindowEx() failed.", "Error", MB_OK );
		exit( 1 );
	}

	OutputDebugString( "***ShowWindow\n" );
	ShowWindow( hWnd, nCmdShow );

	OutputDebugString( "***Wx::MessageLoop\n" );
	return Wx::MessageLoop( hWnd, Update );
}




/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

static LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{ 
	switch( uMsg )
	{
	case WM_CREATE:
		OutputDebugString( "***WM_CREATE\n" );
		InitializeRendering( hWnd );
		InitializeApp();
		return 0;

	case WM_SIZE:
		OutputDebugString( "***WM_SIZE\n" );
		Reshape( hWnd, LOWORD( lParam ), HIWORD( lParam ) );
		return 0;

//	case WM_PAINT:
//		Display();
//		ValidateRect( hWnd, NULL );
//		return 0;

	case WM_CHAR:
	{
		switch ( wParam )
		{
		case VK_ESCAPE:	// Quit
			PostMessage( hWnd, WM_CLOSE, 0, 0 );
			break;

		case ' ':	// Forward
			s_pCamera->Move( Dxx::Vector3ZAxis() * s_CameraSpeed );
			break;

		case 's':	// Backwards
			s_pCamera->Move( -Dxx::Vector3ZAxis() * s_CameraSpeed );
			break;

		case 'd':	// Strafe right
			s_pCamera->Move( Dxx::Vector3XAxis() * s_CameraSpeed );
			break;

		case 'a':	// Strafe left
			s_pCamera->Move( -Dxx::Vector3XAxis() * s_CameraSpeed );
			break;

		case 'w':	// Strafe up
			s_pCamera->Move( Dxx::Vector3YAxis() * s_CameraSpeed );
			break;

		case 'x':	// Strafe down
			s_pCamera->Move( -Dxx::Vector3YAxis() * s_CameraSpeed );
			break;
		}

		return 0;
	}

	case WM_KEYDOWN:
		switch ( wParam )
		{
		case VK_UP:
			s_pCamera->Turn( -.1f, Dxx::Vector3XAxis() );
			break;

		case VK_DOWN:
			s_pCamera->Turn( .1f, Dxx::Vector3XAxis() );
			break;

		case VK_LEFT:
			s_pCamera->Turn( -.1f, Dxx::Vector3YAxis() );
			break;

		case VK_RIGHT:
			s_pCamera->Turn( .1f, Dxx::Vector3YAxis() );
			break;

		case VK_PRIOR:
			s_pCamera->Turn( -.1f, Dxx::Vector3ZAxis() );
			break;

		case VK_INSERT:
			s_pCamera->Turn( .1f, Dxx::Vector3ZAxis() );
			break;
		}
		return 0;

	case WM_CLOSE:
		OutputDebugString( "***WM_CLOSE\n" );
		ShutDownApp();
		ShutDownRendering();
		DestroyWindow( hWnd );
		return 0;

	case WM_DESTROY:
		OutputDebugString( "***WM_DESTROY\n" );
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam ); 
} 


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

static bool Update( HWND hWnd )
{
	__int64 const	t	= Clock::GetTime();

	s_pHud->Update( t );

	Display();

	return true;	// Call this function as often as possible
}




/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

static void Display()
{
	HRESULT		hr;

	// Clear the viewport

	hr = s_pD3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0xff404040, 1.0f, 0 );
	assert_succeeded( hr );

	// Begin the scene

	hr = s_pD3dDevice->BeginScene();
	assert_succeeded( hr );

	// Place the camera

	s_pCamera->Look();

	D3DXVECTOR3 x = s_pCamera->GetDirection();

	// Draw the skybox

	s_pSkyBox->Draw( *s_pCamera );

	hr = s_pD3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	assert_succeeded( hr );

	hr = s_pD3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	assert_succeeded( hr );

	hr = s_pD3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	assert_succeeded( hr );

	s_pLight->Enable();

	DrawTeapot();
	DrawSphere();

	// Draw the hud

	s_pHud->Draw();

	hr = s_pD3dDevice->EndScene();
	assert_succeeded( hr );

	hr = s_pD3dDevice->Present(	NULL, NULL,	NULL, NULL );
	assert_succeeded( hr );
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/


static void Reshape( HWND hWnd, int w, int h )
{
//	HRESULT		hr;
//
//	OutputDebugString( "***ResetD3dWindowedDevice\n" );
//	hr = Dxx::ResetD3dWindowedDevice( s_pD3dDevice, D3DFMT_D24S8 );
//	assert_succeeded( hr );

	OutputDebugString( "***s_pCamera->Reshape\n" );
	s_pCamera->Reshape( w, h );
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

static void InitializeRendering( HWND hWnd )
{
	OutputDebugString( "***InitializeRendering\n" );
	HRESULT		hr;

	// Create the D3D and D3DDevice objects

	s_pD3d = Direct3DCreate9( D3D_SDK_VERSION );
	hr = Dxx::CreateD3dWindowedDevice( s_pD3d, hWnd, &s_pD3dDevice, D3DFMT_D24S8 ); //, D3DADAPTER_DEFAULT, D3DPRESENT_INTERVAL_IMMEDIATE ); //, D3DDEVTYPE_REF  );
	assert_succeeded( hr );

	// Query the device's caps

	s_pD3dDevice->GetDeviceCaps( &s_Caps );

	hr = s_pD3dDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 0.25, 0.27, 0.30, 1.0 ) );
	assert_succeeded( hr );

	// Make sure the necssary caps are supported

//	assert( s_Caps.VertexShaderVersion >= D3DVS_VERSION( 1, 1 ) );	// vs 1.1
//	assert( s_Caps.PixelShaderVersion >= D3DVS_VERSION( 1, 1 ) );	// ps 1.1
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

static void ShutDownRendering()
{
	OutputDebugString( "***ShutDownRendering\n" );

	SAFE_RELEASE( s_pD3dDevice );
	SAFE_RELEASE( s_pD3d );
}



/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

static void InitializeApp()
{
	HRESULT		hr;

	OutputDebugString( "***InitializeApp\n" );

	// Make sure QueryPerformanceCounter is supported

	assert( Clock::IsSupported() );

	// Create the HUD

	s_pHud = new Hud( s_pD3dDevice );

	// Create the camera

	s_pCamera	= new Dxx::Camera( s_pD3dDevice,
								   float( Math::PI_OVER_2 ),
								   1.f, 1000.f,
								   Dxx::Vector3Origin() );

	// Create a light

	s_pLight = new Dxx::DirectionalLight( s_pD3dDevice, 0,
										  Dxx::Sunlight(),
										  Dxx::NoColor(),
										  Dxx::NoColor(),
										  D3DXVECTOR3( -0.6f, -0.4f, -0.6f ) );

	// Create the skybox

	s_pSkyBox = new DxObjects::SkyBox( s_pD3dDevice, "res/Skybox", ".tga" );

	// Create the reference models

	hr = D3DXCreateTeapot( s_pD3dDevice, &s_pTeapot, NULL );
	assert_succeeded( hr );

	hr = D3DXCreateSphere( s_pD3dDevice, 100.0f, 16, 8, &s_pSphere, NULL );
	assert_succeeded( hr );
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

static void ShutDownApp()
{
	OutputDebugString( "***ShutDownApp\n" );

	SAFE_RELEASE( s_pSphere );
	SAFE_RELEASE( s_pTeapot );

	delete s_pSkyBox;
	s_pSkyBox = 0;

	delete s_pLight;
	s_pLight = 0;

	delete s_pCamera;
	s_pCamera = 0;

	delete s_pHud;
	s_pHud = 0;
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

static void DrawTeapot()
{
	HRESULT		hr;
	D3DXMATRIX	world;

	// By default, the teapot's up is down the Y-axis. Rotate it PI/2 around the X-axis to fix that.

	D3DXMatrixRotationX( &world, (float)Math::PI_OVER_2 );

	hr = s_pD3dDevice->SetTransform( D3DTS_WORLD, &world );
	assert_succeeded( hr );

	hr = s_pD3dDevice->SetMaterial( &s_TeapotMaterial );
	assert_succeeded( hr );

	hr = s_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	assert_succeeded( hr );

	hr = s_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
	assert_succeeded( hr );

	hr = s_pTeapot->DrawSubset( 0 );
	assert_succeeded( hr );
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

static void DrawSphere()
{
	HRESULT		hr;
	D3DXMATRIX	world;

	// Put the sphere at (0,0,0).

	D3DXMatrixIdentity( &world );

	hr = s_pD3dDevice->SetTransform( D3DTS_WORLD, &world );
	assert_succeeded( hr );

	hr = s_pD3dDevice->SetMaterial( &s_SphereMaterial );
	assert_succeeded( hr );

	hr = s_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	assert_succeeded( hr );

	hr = s_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
	assert_succeeded( hr );

	hr = s_pD3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	assert_succeeded( hr );

	hr = s_pD3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	assert_succeeded( hr );

	hr = s_pSphere->DrawSubset( 0 );
	assert_succeeded( hr );

	hr = s_pD3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	assert_succeeded( hr );

	hr = s_pD3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	assert_succeeded( hr );

}
