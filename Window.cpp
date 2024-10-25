

#include "Window.h"

#include <gl/gl.h>

using namespace Arkanoid;

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
			window->ProcessLeftClick((int)(lParam & 0xFFFF), (int)(lParam >> 16));
			break;
		case WM_KEYDOWN:
			window->ProcessKeyDown((unsigned)wParam);
			break;
		case WM_KEYUP:
			if (wParam == VK_ESCAPE)
				PostQuitMessage(0);
			else
				window->ProcessKeyUp((unsigned)wParam);
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
	wc.lpszClassName = className;
	return (bool)RegisterClass(&wc);
}

void Window::Unregister()
{
	UnregisterClass(className, GetModuleHandle(NULL));
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
	}
	return L"";
}

Window::Window()
	: hWnd(NULL),
		hDC(NULL),
		hRC(NULL)
{
	window = this;
}

Window::~Window()
{
	Destroy();
}


Window::Error Window::Create(const wchar_t* pTitle, int iW, int iH)
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
					className,
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

	return NONE;
}

void Window::Destroy()
{
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

void Window::Show()
{
	ShowWindow(hWnd, SW_SHOW);
}

void Window::Hide()
{
	ShowWindow(hWnd, SW_HIDE);
}

void Window::Init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Window::Resize(int iW, int iH)
{
	glViewport(0, 0, iW, iH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, iW, iH, 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Window::Update()
{
}

void Window::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
		glColor3f(0.2f, 0.2f, 0.2f);
		glVertex2d(10.0, 0.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2d(1014.0, 0.0);
		glColor3f(0.2f, 0.2f, 0.2f);
		glVertex2d(1014.0, 200.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2d(10.0, 200.0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor4d(1.0, 0.0, 0.0, 0.2);
		glVertex2d(400.0, 50.0);
		glVertex2d(600.0, 50.0);
		glVertex2d(600.0, 500.0);
		glVertex2d(400.0, 500.0);
	glEnd();

	SwapBuffers(hDC);
}

void Window::ProcessLeftClick(int xPos, int yPos)
{
}

void Window::ProcessKeyDown(unsigned keyCode)
{
	switch (keyCode)
	{
	case VK_LEFT:
		break;
	case VK_RIGHT:
		break;
	default:
		break;
	}
}

void Window::ProcessKeyUp(unsigned keyCode)
{
	switch (keyCode)
	{
	case VK_LEFT:
		break;
	case VK_RIGHT:
		break;
	case VK_SPACE:
		break;
	default:
		break;
	}
}
