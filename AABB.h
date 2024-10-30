#pragma once

#include "Geometry.h"

namespace Arkanoid
{
	class AABB
	{
		unsigned GetCode(const Geometry::Point& point) const;
	public:
		AABB() {}
		virtual ~AABB() {}

		virtual double MinX() const = 0;
		virtual double MinY() const = 0;
		virtual double MaxX() const = 0;
		virtual double MaxY() const = 0;

		bool Inside(const Geometry::Point& p1, const Geometry::Point& p2) const;
		bool Outside(const Geometry::Point& p1, const Geometry::Point& p2) const;

		bool InnerIntersection(
			const Geometry::Point& p1,
			const Geometry::Point& p2,
			Geometry::Point& intersection,
			Geometry::Vector& normal) const;
		bool OuterIntersection(
			const Geometry::Point& p1,
			const Geometry::Point& p2,
			Geometry::Point& intersection,
			Geometry::Vector& normal) const;
	};
}