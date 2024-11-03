
#include "Texts.h"

#include <gl/gl.h>

#include <stdio.h>
#include <cstdarg>

using namespace Arkanoid;

constexpr auto FONTNAME = L"Arial";// L"Courier New";

HFONT Texts::hFont = NULL;
unsigned Texts::fontBase = 0;
ABC Texts::glyphInfo[256];

bool Texts::Create(HDC dc)
{
	hFont = CreateFont((int)fontHeight,
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
		FONTNAME);
	if (!hFont)
		return false;
	SelectObject(dc, hFont);

	fontBase = glGenLists(256);
	if (!wglUseFontBitmaps(dc, 0, 255, fontBase))
		return false;

	GetCharABCWidths(dc, 0, 255, &glyphInfo[0]);

	return true;
}

void Texts::Destroy()
{
	if (fontBase)
	{
		glDeleteLists(fontBase, 256);
		fontBase = 0;
	}

	if (hFont)
	{
		DeleteObject(hFont);
		hFont = NULL;
	}
}

int Texts::GetTextWidth(const char* fmt, ...)
{
	if (!(fontBase && fmt))
		return 0;

	char text[256];
	std::va_list args;
	va_start(args, fmt);
	vsnprintf(text, 256, fmt, args);
	va_end(args);

	int w = 0;
	for (int i = 0; i < strlen(text); i++)
		w += glyphInfo[text[i]].abcA + glyphInfo[text[i]].abcB + glyphInfo[text[i]].abcC;

	return w;
}

void Texts::DrawText(double x, double y, const char* fmt, ...)
{
	if (!(fontBase && fmt))
		return;

	char text[256];
	std::va_list args;
	va_start(args, fmt);
	vsnprintf(text, 256, fmt, args);
	va_end(args);

	glRasterPos2d(x, y);
	glPushAttrib(GL_LIST_BIT);
		glListBase(fontBase);
		glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}
