
#ifndef WINDOWH

#define WINDOWH

#include <windows.h>

namespace Arkanoid
{

	class Window
	{
		static constexpr auto className = L"arkanoid-window";

		HWND	hWnd;
		HDC		hDC;
		HGLRC	hRC;
	public:
		enum Error
		{
			CREATEWINDOW,
			GETDC,
			CHOOSEPIXELFORMAT,
			SETPIXELFORMAT,
			CREATERC,
			MAKECURRENTRC,

			NONE
		};

		static bool Register();
		static void Unregister();
		static const wchar_t* GetErrorText(Error errorId);

		Window();
		~Window();

		Error Create(const wchar_t * pTitle, int iW, int iH);
		void Destroy();
		void Show();
		void Hide();
		void Init();
		void Resize(int iW, int iH);
		void Update();
		void Draw();

		void LeftClick(int xPos, int yPos);
		void KeyDown(unsigned keyCode);
		void KeyUp(unsigned keyCode);
	};

}
#endif

