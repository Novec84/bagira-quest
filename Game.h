#pragma once

#include "Texts.h"
#include "Geometry.h"
#include "AABB.h"
#include "Ball.h"
#include "Reflector.h"
#include "Brick.h"

namespace Arkanoid
{
	class Game : public AABB
	{
	public:
		static constexpr double footerPadding = 5.0;
		static constexpr double footerHeight =
			((2.0 * Ball::radius > Texts::fontHeight) ? 2 * Ball::radius : Texts::fontHeight) + 2.0 * footerPadding;

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

		void PrepareToStart(bool restart);
		void ProcessMove();
		void DrawBackground() const;
		void DrawFooter() const;
		void DrawInfo() const;
		void DrawCenteredText(double y, const char* text, ...) const;
	public:
		Game();
		virtual ~Game();

		double MinX() const override { return Ball::radius; }
		double MinY() const override { return Ball::radius; }
		double MaxX() const override { return width - Ball::radius; }
		double MaxY() const override { return height - Ball::radius - Reflector::height - footerHeight; }

		void Init(int iW, int iH);
		void Resize(int iW, int iH);
		void Update();
		void Draw() const;

		void HandleSpacePress();
		void HandleLeftClick(int xPos, int yPos);
		void SetReflectorDirection(double newDirection) { reflector.direction = newDirection; }
	};
}