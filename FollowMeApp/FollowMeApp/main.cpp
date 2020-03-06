#include <windows.h>
#include "CanvasTool.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdline, int  show)
{
	CanvasTool dt;
	dt.create(hInstance, 800, 600, 100, true);

	return 0;
}
