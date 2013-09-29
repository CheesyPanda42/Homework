#include <iostream>
#include <cstdlib>
#include <Windows.h>

using namespace std;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MessageBox(NULL, L"Hello world!", L"Another Hello World Program", MB_ICONEXCLAMATION | MB_OK);

	return 0;
}