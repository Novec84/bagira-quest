#include "Brick.h"
#include "Geometry.h"

#include <windows.h>
#include <gl/GL.h>

using namespace Arkanoid;

constexpr int COLUMNCOUNT = 7;
constexpr int ROWCOUNT = 4;
constexpr double BRICKPADDING = 5;

double Brick::width = 0.0;
double Brick::height = Bricks::height / ROWCOUNT - 2*Ball::radius;

void Brick::Draw() const
{
	double xpos = x * (width + 2*Ball::radius) + Ball::radius;
	double ypos = y * (height + 2*Ball::radius) + Ball::radius;
	glColor3d(red, green, blue);
	glBegin(GL_POLYGON);
		glVertex2d(xpos, ypos);
		glVertex2d(xpos + width, ypos);
		glVertex2d(xpos + width, ypos + height);
		glVertex2d(xpos, ypos + height);
	glEnd();
}

void Bricks::SetWidth(double newWidth)
{
	width = newWidth;
	Brick::width = newWidth / COLUMNCOUNT - 2*Ball::radius;
}

void Bricks::Generate()
{
	const int choiceCount = 3;
	int score[choiceCount] = {
		10,
		20,
		50
	};
	double color[choiceCount][3] = {
		{ 0.80, 0.50, 0.20 }, // bronze
		{ 0.75, 0.75, 0.75 }, // silver
		{ 1.00, 0.84, 0.00 }  // gold
	};
	clear();
	for (int x = 0; x < COLUMNCOUNT; x++)
		for (int y = 0; y < ROWCOUNT; y++)
		{
			int choice = rand() % choiceCount;
			push_back(Brick(x, y, score[choice], color[choice][0], color[choice][1], color[choice][2]));
		}
}

int Bricks::ProccessMove(Ball& ball)
{
	Geometry::Vector normal;
	Geometry::Point intersection;
	Geometry::Point destination = ball.Destination();
	if (Outside(ball.position, destination))
		return 0;
	for (iterator it = begin(); it != end(); it++)
		if (it->OuterIntersection(ball.position, destination, intersection, normal))
		{
			ball.position = intersection;
			ball.direction = Geometry::Reflection(ball.direction, normal);
			int s = it->score;
			erase(it);
			return s;
		}
	return 0;
}

void Bricks::Draw() const
{
	for (auto it : *this)
		it.Draw();
}
