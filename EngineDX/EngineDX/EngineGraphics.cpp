#include "EngineGraphics.h"






// global declarations
LPDIRECT3D9 d3d;								// pointer to d3d interface
LPDIRECT3DDEVICE9 d3ddev;						// pointer to device class
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;		// pointer to vertex buffer


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

	init_graphics();

	d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);		// turn off 3d lighting
}

// function to render a frame
void render_frame(void)
{
	int r,g,b;
	r = 0;
	g = 0;
	b = 0;

	// clear window to blue
	d3ddev -> Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(r,g,b), 1.0f, 0);

	// begin the 3d scene
	d3ddev -> BeginScene();

	// do rendering on back buffer
	// select vertex format
	d3ddev -> SetFVF(CUSTOMFVF);

	// set up pipeline
	D3DXMATRIX matRotateY;

	static float index = 0.0f; 
	index += 0.5f;

	D3DXMatrixRotationY(&matRotateY, index);

	d3ddev -> SetTransform (D3DTS_WORLD, &matRotateY);

	D3DXMATRIX matView; 

	D3DXMatrixLookAtLH ( &matView,
						 &D3DXVECTOR3 (0.0f, 0.0f, 10.0f),    // the camera position
						 &D3DXVECTOR3 (0.0f, 0.0f, 0.0f),    // the look-at position
						 &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction


	d3ddev->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProjection;

	D3DXMatrixPerspectiveFovLH(&matProjection,
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)1400 / (FLOAT)900, // aspect ratio
                               1.0f,    // the near view-plane
                               100.0f);    // the far view-plane

    d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection

	// select vertex buffer to display
	d3ddev -> SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));

	// copy vertex buffer to back buffer
	d3ddev-> DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	// end scene
	d3ddev -> EndScene();

	// display created scene
	d3ddev -> Present(NULL, NULL, NULL, NULL);
}



// function to put 3d model into vRam
// probably should become AddTriangle or something similar
void init_graphics (void)
{
	// create vertices
    // create the vertices using the CUSTOMVERTEX struct
    CUSTOMVERTEX vertices[] = 
    {
        { 3.0f, -3.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255), },
        { 0.0f, 3.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0), },
        { -3.0f, -3.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0), },
    };

    // create a vertex buffer interface called v_buffer
    d3ddev->CreateVertexBuffer(3*sizeof(CUSTOMVERTEX),
                               0,
                               CUSTOMFVF,
                               D3DPOOL_MANAGED,
                               &v_buffer,
                               NULL);

	// void pointer
	VOID *pVoid;

	// lock vRam and load verticies
	v_buffer -> Lock(0,0,(void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer -> Unlock();


}




// clean up
void cleanD3D(void)
{
	v_buffer ->Release();
	d3ddev->Release();
	d3d->Release();
}