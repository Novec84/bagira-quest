#include "Game.h"

#include <gl/gl.h>

#include <cstdarg>

using namespace Arkanoid;

constexpr int BALLCOUNT = 5;
constexpr int STARTANGLE = 30;
constexpr int ENDANGLE = 150;

Game::Game()
	: AABB(),
		state(PREPARED),
		ball(),
		reflector(),
		bricks(),
		width(0.0),
		height(0.0),
		balls(BALLCOUNT),
		score(0),
		fontBase(0),
		glyphInfo(NULL)
{
}

Game::~Game()
{
}

void Game::PrepareToStart(bool restart)
{
	ball.position = Geometry::Point(width / 2.0, height - footerHeight - Ball::radius - Reflector::height);
	ball.direction = Geometry::Vector(0.0, 0.0);

	reflector.position = Geometry::Point((width - Reflector::width) / 2.0, height - footerHeight - Reflector::height);
	reflector.direction = 0.0;

	if (restart)
	{
		bricks.Generate();

		balls = BALLCOUNT;
		score = 0;
	}
}

void Game::Init(int iW, int iH, unsigned uFontBase, const ABC* pGlyphInfo)
{
	width = iW;
	height = iH;
	fontBase = uFontBase;
	glyphInfo = pGlyphInfo;
	bricks.SetWidth(iW);
	state = PREPARED;
	PrepareToStart(true);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Game::Resize(int iW, int iH)
{
	width = iW;
	height = iH;
	bricks.SetWidth(iW);

	glViewport(0, 0, iW, iH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, iW, iH, 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Game::Update()
{
	if (state != PLAYING)
		return;

	reflector.UpdatePosition(width);

	int s = bricks.ProccessMove(ball);
	if (s > 0)
	{
		score += s;
		if (bricks.empty())
			state = WIN;
		return;
	}

	if (reflector.ProccessMove(ball))
		return;

	ProcessMove();
}

void Game::ProcessMove()
{
	Geometry::Vector normal;
	Geometry::Point intersection;
	Geometry::Point destination = ball.Destination();
	if (InnerIntersection(ball.position, destination, intersection, normal))
	{
		if (normal.y == -1.0)
		{
			balls--;
			if (balls <= 0)
				state = LOOSE;
			else
			{
				PrepareToStart(false);
				ball.direction = Geometry::RandomDirection(STARTANGLE, ENDANGLE);
				state = PAUSED;
			}
		}
		else
		{
			ball.position = intersection;
			ball.direction = Geometry::Reflection(ball.direction, normal);
		}
	}
	else
		ball.position = destination;
}

void Game::Draw() const
{
	glClear(GL_COLOR_BUFFER_BIT);

	DrawBackground();
	bricks.Draw();
	ball.Draw(true);
	reflector.Draw();
	DrawFooter();
	if (state != PLAYING)
		DrawInfo();
}

void Game::DrawBackground() const
{
	glColor3d(0.0, 0.0, 0.1);
	glBegin(GL_POLYGON);
		glVertex2d(0.0, 0.0);
		glVertex2d(width, 0.0);
		glVertex2d(width, height - footerHeight);
		glVertex2d(0.0, height - footerHeight);
	glEnd();
}

void Game::DrawFooter() const
{
	glColor3d(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2d(0.0, height - footerHeight);
		glVertex2d(width, height - footerHeight);
		glVertex2d(width, height);
		glVertex2d(0.0, height);
	glEnd();

	glColor3d(1.0, 1.0, 1.0);
	DrawString(footerPadding, height - footerPadding, "SCORE: %d", score);

	Ball drawnBall;
	drawnBall.position = Geometry::Point(width - footerPadding - Ball::radius, height - footerHeight / 2.0);
	for (int i = 0; i < BALLCOUNT; i++)
	{
		drawnBall.Draw((BALLCOUNT - i) <= balls);
		drawnBall.position.x -= 2.0 * Ball::radius + footerPadding;
	}
}

void Game::DrawInfo() const
{
	glColor4d(0.0, 0.0, 0.0, 0.8);
	glBegin(GL_POLYGON);
		glVertex2d(0.0, 0.0);
		glVertex2d(width, 0.0);
		glVertex2d(width, height - footerHeight);
		glVertex2d(0.0, height - footerHeight);
	glEnd();

	double infoHeight = 5 * fontHeight; //title + separator + esc keypress + two choices in all cases
	double h = (height - infoHeight) / 2.0;
	
	glColor3d(1.0, 1.0, 0.0);
	DrawCenteredString(h, "ARKANOID");
	h += fontHeight;
	DrawCenteredString(h, "----------------------------------");
	h += fontHeight;
	glColor3d(1.0, 1.0, 1.0);
	switch (state)
	{
	case PAUSED:
		DrawCenteredString(h, "change ball direction by [LEFT CLICK]");
		h += fontHeight;
		DrawCenteredString(h, "press [SPACE] to continue");
		break;
	case PREPARED:
		DrawCenteredString(h, "change ball direcction by [LEFT CLICK]");
		h += fontHeight;
		DrawCenteredString(h, "press [SPACE] to start");
		break;
	case WIN:
		glColor3d(0.0, 0.8, 0.0);
		DrawCenteredString(h, "YOU WIN, your score is %d", score);
		h += fontHeight;
		glColor3d(1.0, 1.0, 1.0);
		DrawCenteredString(h, "press [SPACE] to continue");
		break;
	case LOOSE:
		glColor3d(0.8, 0.0, 0.0);
		DrawCenteredString(h, "YOU LOOSE");
		h += fontHeight;
		glColor3d(1.0, 1.0, 1.0);
		DrawCenteredString(h, "press [SPACE] to continue");
		break;
	}
	h += fontHeight;
	DrawCenteredString(h, "press [ESC] to exit");
}

void Game::DrawString(double xPos, double yPos, const char* fmt, ...) const
{
	if (!(fontBase && glyphInfo && fmt))
		return;

	char text[256];
	std::va_list args;
	va_start(args, fmt);
	vsnprintf(text, 256, fmt, args);
	va_end(args);

	glRasterPos2d(xPos, yPos);
	glPushAttrib(GL_LIST_BIT);
		glListBase(fontBase);
		glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

void Game::DrawCenteredString(double yPos, const char* fmt, ...) const
{
	if (!(fontBase && glyphInfo && fmt))
		return;

	char text[256];
	std::va_list args;
	va_start(args, fmt);
	vsnprintf(text, 256, fmt, args);
	va_end(args);

	int w = 0;
	for (int i = 0; i < strlen(text); i++)
		w += glyphInfo[text[i]].abcA + glyphInfo[text[i]].abcB + glyphInfo[text[i]].abcC;

	DrawString((width - w) / 2.0, yPos, text);
}

void Game::HandleSpacePress()
{
	switch (state)
	{
	case PLAYING:
		state = PAUSED;
		break;
	case PAUSED:
		state = PLAYING;
		break;
	case PREPARED:
		ball.direction = Geometry::RandomDirection(STARTANGLE, ENDANGLE);
		state = PLAYING;
		break;
	case WIN:
		PrepareToStart(true);
		state = PREPARED;
		break;
	case LOOSE:
		PrepareToStart(true);
		state = PREPARED;
		break;
	default:
		break;
	}
}

void Game::HandleLeftClick(int xPos, int yPos)
{
	if (state != PLAYING)
		return;
	ball.direction = Geometry::Vector(ball.position, Geometry::Point(xPos, yPos));
	ball.direction.Normalize();
}
