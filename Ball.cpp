
#include "Ball.h"

#include <windows.h>
#include <gl/gl.h>
#include <cmath>

using namespace Arkanoid;

constexpr int BALLQUALITY = 16;

void Ball::Draw(bool validBall) const
{
	//FIXME: use textures instead of mathematic
	glColor3d(1.0, 1.0, 1.0);
	if (validBall)
	{
		double radStep = 2.0*Geometry::MPI / BALLQUALITY;
		double lastX = radius;
		double lastY = 0.0;;
		int i;
		for (i = 1; i <= BALLQUALITY; i++)
		{
			double actX = cos(radStep * i) * radius;
			double actY = sin(radStep * i) * radius;
			glBegin(GL_TRIANGLES);
				glVertex2d(position.x, position.y);
				glVertex2d(position.x + lastX, position.y + lastY);
				glVertex2d(position.x + actX, position.y + actY);
			glEnd();
			lastX = actX;
			lastY = actY;
		}
	}
	else
	{
		glBegin(GL_LINES);
			glVertex2d(position.x - radius, position.y - radius);
			glVertex2d(position.x + radius, position.y + radius);
			glVertex2d(position.x + radius, position.y - radius);
			glVertex2d(position.x - radius, position.y + radius);
		glEnd();
	}
}