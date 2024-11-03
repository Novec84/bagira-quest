
#include "Window.h"

#include <time.h>

using namespace Arkanoid;

constexpr auto WINDOWTITLE = L"Arkanoid";
constexpr int WINDOWWIDTH = 1024;
constexpr int WINDOWHEIGHT = 768;
constexpr int WINDOWFPS = 20;

int main()
{
	MSG		msg;
	BOOL	bWork = TRUE;
	Window	window;
	Window::Error error;
	int     sleepTime = 1000 / WINDOWFPS;

	srand((unsigned)time(NULL));

	if (!Window::Register())
	{
		MessageBox(NULL, L"Window registration failed", L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	error = window.Create(WINDOWTITLE, WINDOWWIDTH, WINDOWHEIGHT);
	if (error != Window::NONE)
	{
		MessageBox(NULL, Window::GetErrorText(error), L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	window.Init(WINDOWWIDTH, WINDOWHEIGHT);
	window.Show();

	while (bWork)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bWork = FALSE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			window.Update();
			window.Draw();
			Sleep(sleepTime);
		}
	}

	window.Hide();
	window.Destroy();
	Window::Unregister();
	return 0;
}
