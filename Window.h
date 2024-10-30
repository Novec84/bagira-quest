
#pragma once

#include <windows.h>

#include "Game.h"

namespace Arkanoid
{

	class Window
	{
		HWND		hWnd;
		HDC			hDC;
		HGLRC		hRC;
		HFONT		hFont;
		unsigned	uFontBase;
		ABC			glyphInfo[256];

		Game	game;
	public:
		enum Error
		{
			CREATEWINDOW,
			GETDC,
			CHOOSEPIXELFORMAT,
			SETPIXELFORMAT,
			CREATERC,
			MAKECURRENTRC,
			CREATEFONT,
			USEFONT,

			NONE
		};

		static bool Register();
		static void Unregister();
		static const wchar_t* GetErrorText(Error errorId);

		Window();
		~Window();

		Error Create(const wchar_t* pTitle, int iW, int iH, const wchar_t* pFontName);
		void Destroy();
		void Init(int iW, int iH);
		void Show();
		void Hide();
		void Resize(int iW, int iH);
		void Draw();
		void Update();

		void LeftClick(int xPos, int yPos);
		void KeyDown(unsigned keyCode);
		void KeyUp(unsigned keyCode);
	};

}
