#include "EngineGraphics.h"


// global declarations
LPDIRECT3D9 d3d;	// pointer to d3d interface
LPDIRECT3DDEVICE9 d3ddev; // pointer to device class



// initialize graphics
HWND EGSysInit (HINSTANCE hInst, int nCmdShow, int winWidth, int winHeight, LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam), bool fullscreen)
{
	HWND hwnd;

	// window class struct
	WNDCLASSEX wc;

	// clear wc
	ZeroMemory (&wc, sizeof(WNDCLASSEX));

	// fill in struct with info
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if(!fullscreen) wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass1";

	// register window class
	RegisterClassEx(&wc);

	// create window and use result as handle
	if(!fullscreen)
		hwnd = CreateWindowEx (NULL, 
								"WindowClass1",
								"My first Window",
								WS_OVERLAPPEDWINDOW,
								300,
								300,
								winWidth,
								winHeight,
								NULL,
								NULL,
								hInst,
								NULL);
	// create fullscreen window
	else
		hwnd = CreateWindowEx (NULL, 
								"WindowClass1",
								"My first Window",
								WS_EX_TOPMOST | WS_POPUP,
								0,
								0,
								winWidth,
								winHeight,
								NULL,
								NULL,
								hInst,
								NULL);


	// display window
	ShowWindow(hwnd, nCmdShow);

	// initialize Direct3D
	initD3D(hwnd, fullscreen, winHeight, winWidth);

	return hwnd;
}



// initialize Direct 3D for use
void initD3D(HWND hWnd, bool fullscreen, int winHeight, int winWidth)
{
	d3d = Direct3DCreate9 (D3D_SDK_VERSION);			// create Direct3D interface

	D3DPRESENT_PARAMETERS d3dpp;						// struct for various device info
		
	ZeroMemory(&d3dpp, sizeof(d3dpp));					// clear struct

	d3dpp.Windowed = !fullscreen;						// set windowed mode (TRUE_
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;			// discard old frames
	d3dpp.hDeviceWindow = hWnd;							// set window in use

	if (fullscreen)
	{
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;		// back buffer format to 32 bit
		d3dpp.BackBufferWidth = winWidth;				// set width of back buffer
		d3dpp.BackBufferHeight = winHeight;				// set height of back buffer
	}

	// create device class using this info
	d3d->CreateDevice ( D3DADAPTER_DEFAULT,
						D3DDEVTYPE_HAL,
						hWnd,
						D3DCREATE_SOFTWARE_VERTEXPROCESSING,
						&d3dpp,
						&d3ddev);
}

// function to render a frame
void render_frame(void)
{
	int r,g,b;
	r = 0;
	g = 100;
	b = 200;

	// clear window to blue
	d3ddev -> Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(r,g,b), 1.0f, 0);

	// begin the 3d scene
	d3ddev -> BeginScene();

	// do rendering on back buffer
	

	// end scene
	d3ddev -> EndScene();

	// display created scene
	d3ddev -> Present(NULL, NULL, NULL, NULL);
}


// clean up
void cleanD3D(void)
{
	d3ddev->Release();
	d3d->Release();
}