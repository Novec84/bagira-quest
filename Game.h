#pragma once

#include "Geometry.h"
#include "AABB.h"
#include "Ball.h"
#include "Reflector.h"
#include "Brick.h"

#include <Windows.h>

namespace Arkanoid
{
	class Game : public AABB
	{
	public:
		static constexpr double fontHeight = 20.0;
		static constexpr double footerPadding = 5.0;
		static constexpr double footerHeight =
			((2.0 * Ball::radius > fontHeight) ? 2 * Ball::radius : fontHeight) + 2.0 * footerPadding;

		enum
		{
			PLAYING,
			PAUSED,
			PREPARED,
			WIN,
			LOOSE
		} state;
		
		Ball		ball;
		Reflector	reflector;
		Bricks		bricks;

		double	width;
		double	height;
		int		balls;
		int		score;

		unsigned	fontBase;
		const ABC*	glyphInfo;

		void PrepareToStart(bool restart);
		void ProcessMove();
		void DrawBackground() const;
		void DrawFooter() const;
		void DrawInfo() const;
		void DrawString(double xPos, double yPos, const char* text, ...) const;
		void DrawCenteredString(double yPos, const char* text, ...) const;
	public:
		Game();
		virtual ~Game();

		double MinX() const override { return Ball::radius; }
		double MinY() const override { return Ball::radius; }
		double MaxX() const override { return width - Ball::radius; }
		double MaxY() const override { return height - Ball::radius - Reflector::height - footerHeight; }

		void Init(int iW, int iH, unsigned uFontBase, const ABC* pGlyphInfo);
		void Resize(int iW, int iH);
		void Update();
		void Draw() const;

		void HandleSpacePress();
		void HandleLeftClick(int xPos, int yPos);
		void SetReflectorDirection(double newDirection) { reflector.direction = newDirection; }
	};
}