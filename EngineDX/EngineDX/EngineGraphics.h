#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
#include <WindowsX.h>

// include Direct3D library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")



#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
struct CUSTOMVERTEX
{
	float x, y, z;
	DWORD color;
};

// initialize window
HWND EGSysInit( HINSTANCE hInst, 
				int nCmdShow, 
				int winWidth, 
				int winHeight, 
				LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam),
				bool fullscreen);				
// initialize Direct3D
void initD3D (HWND hWnd, bool fullscree, int winHeight, int winWidth);
// draw a frame to screen
void render_frame(void);
// close Direct3D and free memory
void cleanD3D(void);		
// 3D declarations
void init_graphics(void);
