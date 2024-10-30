#pragma once

#include "Geometry.h"

namespace Arkanoid
{
	struct Ball
	{
		static constexpr double radius = 10.0;
		static constexpr double speed = 25.0;

		Geometry::Point		position;
		Geometry::Vector	direction;

		Ball() : position(), direction() {}
		~Ball() {}

		Geometry::Point Destination() const { return position + speed * direction; }

		void Draw(bool validBall) const;
	};
}