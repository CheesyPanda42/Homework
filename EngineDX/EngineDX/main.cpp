#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <WindowsX.h>
#include "EngineGraphics.h"

using namespace std;

// WindowProc function prototype
LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// entry point for windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// window handle
	HWND hWnd;

	hWnd = EGSysInit (hInstance, nCmdShow, 1440, 900, WindowProc, true);

	// enter main loop
	MSG msg;

	while (TRUE)
	{
		while (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		// draw
		render_frame();
	}

	while (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	cleanD3D();

	return msg.wParam;

	return 0;
}

// message handler
LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}