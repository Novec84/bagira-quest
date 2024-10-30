#pragma once

#include "Ball.h"
#include "AABB.h"

#include <list>

namespace Arkanoid
{
	struct Brick : public AABB
	{
		static double width;
		static double height;

		int		x;
		int		y;
		int		score;
		double	red;
		double	green;
		double	blue;

		Brick() : x(0), y(0), score(0), red(0.0), green(0.0), blue(0.0) {}
		Brick(int _x, int _y, int _s, double _r, double _g, double _b)
			: x(_x), y(_y), score(_s), red(_r), green(_g), blue(_b) {}
		~Brick() {}

		double MinX() const override { return x * (width + 2*Ball::radius); }
		double MinY() const override { return y * (height + 2*Ball::radius); }
		double MaxX() const override { return (x + 1) * (width + 2*Ball::radius); }
		double MaxY() const override { return (y + 1) * (height + 2*Ball::radius); }

		void Draw() const;
	};

	struct Bricks : public std::list<Brick>, public AABB
	{
		static constexpr double height = 250.0;

		double	width;

		Bricks() : list<Brick>(), width(0.0) {}
		~Bricks() { clear(); }

		double MinX() const override { return Ball::radius; }
		double MinY() const override { return Ball::radius; }
		double MaxX() const override { return width - Ball::radius; }
		double MaxY() const override { return height - Ball::radius; }

		void SetWidth(double newWidth);
		void Generate();
		int ProccessMove(Ball& ball);
		void Draw() const;
	};
}