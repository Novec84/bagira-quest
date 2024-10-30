

#include "Window.h"

#include <gl/GL.h>

using namespace Arkanoid;

constexpr auto CLASSNAME = L"arkanoid-window";

static Window* window = NULL;

LRESULT CALLBACK WindowFunction(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (window)
		switch (message)
		{
		case WM_PAINT:
			window->Draw();
			ValidateRect(hWnd, NULL);
			return 0;
			break;
		case WM_SIZE:
			window->Resize((int)(lParam & 0xFFFF), (int)(lParam >> 16));
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		case WM_LBUTTONUP:
			window->LeftClick((int)(lParam & 0xFFFF), (int)(lParam >> 16));
			break;
		case WM_KEYDOWN:
			window->KeyDown((unsigned)wParam);
			break;
		case WM_KEYUP:
			if (wParam == VK_ESCAPE)
				PostQuitMessage(0);
			else
				window->KeyUp((unsigned)wParam);
			break;
		default:
			break;
		}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool Window::Register()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WindowFunction;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASSNAME;
	return (bool)RegisterClass(&wc);
}

void Window::Unregister()
{
	UnregisterClass(CLASSNAME, GetModuleHandle(NULL));
}

const wchar_t* Window::GetErrorText(Window::Error errorId)
{
	switch (errorId)
	{
	case CREATEWINDOW:
		return L"Window creation failed";
	case GETDC:
		return L"Cannot get device context";
	case CHOOSEPIXELFORMAT:
		return L"Cannot choose pixel format";
	case SETPIXELFORMAT:
		return L"Cannot set pixel format";
	case CREATERC:
		return L"Resource context creation failed";
	case MAKECURRENTRC:
		return L"Cannot make resource context as current";
	case CREATEFONT:
		return L"Font creation failed";
	case USEFONT:
		return L"Cannot use font";
	}
	return L"";
}

Window::Window()
	: hWnd(NULL),
		hDC(NULL),
		hRC(NULL),
		hFont(NULL),
		uFontBase(0),
		game()
{
	window = this;
	memset(&glyphInfo[0], 0, 256 * sizeof(ABC));
}

Window::~Window()
{
	Destroy();
}


Window::Error Window::Create(const wchar_t* pTitle, int iW, int iH, const wchar_t* pFontName)
{
	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	DWORD	style;
	DWORD	extStyle;
	RECT	wndRect;
	int		pixelFormat;

	style = WS_CAPTION | WS_OVERLAPPED;
	extStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	wndRect.left = 0;
	wndRect.right = iW;
	wndRect.top = 0;
	wndRect.bottom = iH;
	AdjustWindowRectEx(&wndRect, style, FALSE, extStyle);

	hWnd = CreateWindowEx(
					extStyle,
					CLASSNAME,
					pTitle,
					style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
					0, 0, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top,
					NULL,
					NULL,
					GetModuleHandle(NULL),
					NULL);
	if (!hWnd)
		return CREATEWINDOW;

	hDC = GetDC(hWnd);
	if (!hDC)
		return GETDC;

	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (!pixelFormat)
		return CHOOSEPIXELFORMAT;
	if (!SetPixelFormat(hDC, pixelFormat, &pfd))
		return SETPIXELFORMAT;
	hRC = wglCreateContext(hDC);
	if (!hRC)
		return CREATERC;
	if (!wglMakeCurrent(hDC, hRC))
		return MAKECURRENTRC;

	hFont = CreateFont((int)Game::fontHeight,
		0,
		0,
		0,
		FW_BOLD,
		FALSE,// cursive
		FALSE,// underscore
		FALSE,// crossed
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		pFontName);
	if (!hFont)
		return CREATEFONT;
	SelectObject(hDC, hFont);

	uFontBase = glGenLists(256);
	if (!wglUseFontBitmaps(hDC, 0, 255, uFontBase))
		return USEFONT;
	GetCharABCWidths(hDC, 0, 255, &glyphInfo[0]);

	return NONE;
}

void Window::Destroy()
{
	if (uFontBase)
	{
		glDeleteLists(uFontBase, 256);
		uFontBase = 0;
	}

	if (hFont)
	{
		DeleteObject(hFont);
		hFont = NULL;
	}

	if (hRC)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRC);
		hRC = NULL;
	}

	if (hDC)
	{
		ReleaseDC(hWnd, hDC);
		hDC = NULL;
	}

	if (hWnd)
	{
		DestroyWindow(hWnd);
		hWnd = NULL;
	}
}
void Window::Init(int iW, int iH)
{
	game.Init(iW, iH, uFontBase, &glyphInfo[0]);
}

void Window::Show()
{
	ShowWindow(hWnd, SW_SHOW);
}

void Window::Hide()
{
	ShowWindow(hWnd, SW_HIDE);
}

void Window::Resize(int iW, int iH)
{
	game.Resize(iW, iH);
}

void Window::Draw()
{
	game.Draw();
	SwapBuffers(hDC);
}

void Window::Update()
{
	game.Update();
}

void Window::LeftClick(int xPos, int yPos)
{
	game.HandleLeftClick(xPos, yPos);
}

void Window::KeyDown(unsigned keyCode)
{
	switch (keyCode)
	{
	case VK_LEFT:
		game.SetReflectorDirection(-1.0);
		break;
	case VK_RIGHT:
		game.SetReflectorDirection(1.0);
		break;
	default:
		break;
	}
}

void Window::KeyUp(unsigned keyCode)
{
	switch (keyCode)
	{
	case VK_LEFT:
	case VK_RIGHT:
		game.SetReflectorDirection(0.0);
		break;
	case VK_SPACE:
		game.HandleSpacePress();
		break;
	default:
		break;
	}
}
