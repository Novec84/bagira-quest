#pragma once

#include <cmath>

namespace Arkanoid
{
	class Geometry
	{
	public:
		static constexpr double MPI = 3.14159265358979323846;

		struct Vector;

		struct Point
		{
			double x;
			double y;

			Point() : x(0.0), y(0.0) {}
			Point(double _x, double _y) : x(_x), y(_y) {}
			Point(const Point& p) : x(p.x), y(p.y) {}

			static double Distance(const Point& p1, const Point& p2)
			{ return sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y)); }

			Point& operator+=(const Vector& v) { x += v.x; y += v.y; return *this; }
		};

		struct Vector
		{
			double x;
			double y;

			Vector() : x(0.0), y(0.0) {}
			Vector(double _x, double _y) : x(_x), y(_y) {}
			Vector(const Point& p1, const Point& p2) : x(p2.x - p1.x), y(p2.y - p1.y) {}
			Vector(const Vector& v) :x(v.x), y(v.y) {}

			bool IsZero() const { return (x == 0.0) && (y == 0.0); }
			double Length() const { return sqrt(x * x + y * y); }
			void Normalize() { double l = Length(); if (l != 0.0) { x /= l; y /= l; } else x = y = 0.0; }
			static double DotProduct(const Vector& v1, const Vector& v2) { return v1.x * v2.x + v1.y * v2.y; }

			Vector& operator-=(const Vector& v) { x -= v.x; y -= v.y; return *this; }

			friend Point operator+(const Point& p, const Vector& v) { return Point(p.x + v.x, p.y + v.y); }
			friend Point operator+(const Vector& v, const Point& p) { return Point(p.x + v.x, p.y + v.y); }
			friend Vector operator*(const Vector& v, double l) { return Vector(v.x * l, v.y * l); }
			friend Vector operator*(double l, const Vector& v) { return Vector(v.x * l, v.y * l); }
			friend Vector operator+(const Vector& v1, const Vector& v2) { return Vector(v1.x + v2.x, v1.y + v2.y); }
			friend Vector operator-(const Vector& v1, const Vector& v2) { return Vector(v1.x - v2.x, v1.y - v2.y); }
		};

		static Vector Reflection(const Vector& vector, const Vector& normal)
		{
			return vector - 2.0 * Vector::DotProduct(vector, normal) * normal;
		}

		static Vector RandomDirection(int startAngle, int endAngle)
		{
			double rad = (startAngle + (rand()%(endAngle - startAngle + 1))) * MPI / 180.0;
			return Vector(cos(rad), -sin(rad));
		}
	};
}
