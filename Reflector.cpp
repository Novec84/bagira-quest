
#include "Reflector.h"

#include <windows.h>
#include <gl/GL.h>

using namespace Arkanoid;

constexpr double REFLECTORSPEED = 25.0;

void Reflector::UpdatePosition(double totalWidth)
{
	position.x += direction * REFLECTORSPEED;
	if (position.x < 0)
		position.x = 0;
	if (position.x > totalWidth - width)
		position.x = totalWidth - width;
}

bool Reflector::ProccessMove(Ball& ball) const
{
	Geometry::Vector normal;
	Geometry::Point intersection;
	Geometry::Point destination = ball.Destination();
	if (OuterIntersection(ball.position, destination, intersection, normal))
	{
		if (normal.y == -1.0)
		{
			ball.position = intersection;
			ball.direction = Geometry::Reflection(ball.direction, normal);
			return true;
		}
	}
	return false;
}

void Reflector::Draw() const
{
	glBegin(GL_POLYGON);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glVertex2d(position.x, position.y);
		glVertex2d(position.x + width, position.y);
		glColor4d(1.0, 1.0, 1.0, 0.0);
		glVertex2d(position.x + width, position.y + height);
		glVertex2d(position.x, position.y + height);
	glEnd();
}