#pragma once

#include "Geometry.h"
#include "AABB.h"
#include "Ball.h"

namespace Arkanoid
{
	class Reflector : public AABB
	{
	public:
		static constexpr double width = 150.0;
		static constexpr double height = 20.0;

		Geometry::Point	position;
		double			direction;

		Reflector() : AABB(), position(), direction(0.0) {}
		~Reflector() {}

		double MinX() const override { return position.x - Ball::radius; }
		double MinY() const override { return position.y - Ball::radius; }
		double MaxX() const override { return position.x + width + Ball::radius; }
		double MaxY() const override { return position.y + height + Ball::radius; }

		void UpdatePosition(double totalWidth);
		bool ProccessMove(Ball& ball) const;
		void Draw() const;
	};
}