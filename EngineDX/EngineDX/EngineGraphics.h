#include <d3d9.h>

// include Direct3D library file
#pragma comment (lib, "d3d9.lib")



// function proto

HWND EGSysInit( HINSTANCE hInst, 
				int nCmdShow, 
				int winWidth, 
				int winHeight, 
				LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam),
				bool fullscreen);				// initialize window

void initD3D (HWND hWnd, bool fullscree, int winHeight, int winWidth);		// initialize d3d
void render_frame(void);		// render a single frame
void cleanD3D(void);			// close D3D and free mem

