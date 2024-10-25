
#include "Window.h"

using namespace Arkanoid;

constexpr auto WINDOWTITLE = L"Arkanoid";
constexpr auto WINDOWWIDTH = 1024;
constexpr auto WINDOWHEIGHT = 768;
constexpr auto WINDOWFPS = 20;

int main()
{
	MSG		msg;
	BOOL	bWork = TRUE;
	Window	window;
	Window::Error error;
	int     sleepTime = 1000 / WINDOWFPS;

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

	window.Init();
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
