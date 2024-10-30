
#include "AABB.h"

using namespace Arkanoid;

//Cohen–Sutherland

constexpr unsigned INSIDE =	0b0000;
constexpr unsigned LEFT =	0b0001;
constexpr unsigned RIGHT =	0b0010;
constexpr unsigned TOP =	0b0100;
constexpr unsigned BOTTOM =	0b1000;

unsigned AABB::GetCode(const Geometry::Point& point) const
{
	unsigned code = INSIDE;
	if (point.x < MinX())
		code |= LEFT;
	if (point.x > MaxX())
		code |= RIGHT;
	if (point.y < MinY())
		code |= TOP;
	if (point.y > MaxY())
		code |= BOTTOM;
	return code;
}

bool AABB::Inside(const Geometry::Point& p1, const Geometry::Point& p2) const
{
	return !(GetCode(p1) | GetCode(p2));
}

bool AABB::Outside(const Geometry::Point& p1, const Geometry::Point& p2) const
{
	return GetCode(p1) & GetCode(p2);
}

bool AABB::InnerIntersection(
	const Geometry::Point& p1,
	const Geometry::Point& p2,
	Geometry::Point& intersection,
	Geometry::Vector& normal) const
{
	unsigned code = GetCode(p1);
	// point have to be in
	if (code)
		return false;
	bool intersected = false;
	intersection = p2;
	for (;;)
	{
		code = GetCode(intersection);
		// intersection is in
		if (!code)
			break;
		double x0 = p1.x;
		double y0 = p1.y;
		double x1 = intersection.x;
		double y1 = intersection.y;
		if (code & TOP)
		{
			intersection.x = x0 + (x1 - x0) * (MinY() - y0) / (y1 - y0);
			intersection.y = MinY();
			normal.x = 0.0;
			normal.y = 1.0;
			intersected = true;
		}
		else
		if(code & BOTTOM)
		{
			intersection.x = x0 + (x1 - x0) * (MaxY() - y0) / (y1 - y0);
			intersection.y = MaxY();
			normal.x = 0.0;
			normal.y = -1.0;
			intersected = true;
		}
		else
		if(code & LEFT)
		{
			intersection.x = MinX();
			intersection.y = y0 + (y1 - y0) * (MinX() - x0) / (x1 - x0);
			normal.x = 1.0;
			normal.y = 0.0;
			intersected = true;
		}
		else
		if(code & RIGHT)
		{
			intersection.x = MaxX();
			intersection.y = y0 + (y1 - y0) * (MaxX() - x0) / (x1 - x0);
			normal.x = -1.0;
			normal.y = 0.0;
			intersected = true;
		}
	}
	return intersected;
}

bool AABB::OuterIntersection(
	const Geometry::Point& p1,
	const Geometry::Point& p2,
	Geometry::Point& intersection,
	Geometry::Vector& normal) const
{
	unsigned code0 = GetCode(p1);
	//point have to be out
	if (!code0)
		return false;
	Geometry::Vector n0, n1;
	double x0 = p1.x;
	double y0 = p1.y;
	double x1 = p2.x;
	double y1 = p2.y;
	unsigned code1 = GetCode(p2);
	for (;;)
	{
		if ((!(code0 | code1)) || (code0 & code1)) // both inside or outside
			break;
		double x = 0.0;
		double y = 0.0;
		double nx = 0.0;
		double ny = 0.0;
		unsigned code = (code1 > code0) ? code1 : code0;
		if (code & TOP)
		{
			x = x0 + (x1 - x0) * (MinY() - y0) / (y1 - y0);
			y = MinY();
			nx = 0.0;
			ny = -1.0;
		}
		else
		if (code & BOTTOM)
		{
			x = x0 + (x1 - x0) * (MaxY() - y0) / (y1 - y0);
			y = MaxY();
			nx = 0.0;
			ny = 1.0;
		}
		else
		if (code & LEFT)
		{
			x = MinX();
			y = y0 + (y1 - y0) * (MinX() - x0) / (x1 - x0);
			nx = -1.0;
			ny = 0.0;
		}
		else
		if(code & RIGHT)
		{
			x = MaxX();
			y = y0 + (y1 - y0) * (MaxX() - x0) / (x1 - x0);
			nx = 1.0;
			ny = 0.0;
		}
		if (code == code0)
		{
			x0 = x;
			y0 = y;
			n0.x = nx;
			n0.y = ny;
			code0 = GetCode(Geometry::Point(x0, y0));
		}
		else
		{
			x1 = x;
			y1 = y;
			n1.x = nx;
			n1.y = ny;
			code1 = GetCode(Geometry::Point(x1, y1));
		}
	}
	if (n0.IsZero() && n1.IsZero())
		return false;
	if (n1.IsZero())
	{
		intersection.x = x0;
		intersection.y = y0;
		normal = n0;
		return true;
	}
	if (n0.IsZero())
	{
		intersection.x = x1;
		intersection.y = y1;
		normal = n1;
		return true;
	}
	double d0 = Geometry::Point::Distance(p1, Geometry::Point(x0, y0));
	double d1 = Geometry::Point::Distance(p1, Geometry::Point(x1, y1));
	if (d1 < d0)
	{
		intersection.x = x1;
		intersection.y = y1;
		normal = n1;
	}
	else
	{
		intersection.x = x0;
		intersection.y = y0;
		normal = n0;
	}
	return true;
}
