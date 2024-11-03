#pragma once

#include <Windows.h>

namespace Arkanoid
{
	class Texts
	{
		static HFONT	hFont;
		static unsigned fontBase;
		static ABC		glyphInfo[256];
	public:
		static constexpr double fontHeight = 20.0;

		Texts() = delete;

		static bool Create(HDC dc);
		static void Destroy();

		static int GetTextWidth(const char* fmt, ...);
		static void DrawText(double x, double y, const char* fmt, ...);
	};
}
