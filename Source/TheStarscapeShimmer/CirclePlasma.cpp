#include "TheStarscapeShimmer.h"
#include "ProceduralFunc.h"
#include <cmath>

// Fuzzy circle with radius .5 centered at (.5, .5).
float circle(Vec2* p)
{
	float r = .5;
	Vec2 center = { r, r };
	float d = sqrtf((p->X - r) * (p->X - r) + (p->Y - r) * (p->Y - r));
	return step(d, r) * pow(1.0f - d, 4);
}

void CirclePlasma(uint8_t* Colours, Vec2* Pos, Vec2* Bounds, double ElapsedTime)
{
	
	float time = (float)(ElapsedTime * .2);
	Vec2 uv = { -1.0 + 2.0 * Pos->X / Bounds->X, -1.0 + 2.0 * Pos->Y / Bounds->Y };
	uv.X *= Bounds->X / Bounds->Y;

	// Move up and zoom in..
	uv.Y += 1.f;
	uv.X *= 0.5;
	uv.Y *= 0.5;

	float sets[5];

	// First set:
	Vec2 p = uv;
	p.X += sin(3.*p.X)*sin(2.*p.X + time);
	p.X = fmodf(p.X, 0.2) * 7;
	p.Y = fmodf(p.Y, 0.2) * 7;
	sets[0] = circle(&p);

	// Second set:
	p = uv;
	p.Y += sin(10.*p.Y)*sin(2.3*p.X + .8*time);
	p.X = fmodf(p.X * 0.6, 0.2) * 7;
	p.Y = fmodf(p.Y * 0.6, 0.2) * 7;
	sets[1] = circle(&p);

	// Third set:
	p = uv;
	p.Y += sin(10.*p.X)*sin(2.3*p.Y + 2.*time);
	p.X += sin(p.Y)*sin(p.X) + sin(time);
	p.X = fmodf(p.X * 0.4, 0.2) * 4;
	p.Y = fmodf(p.Y * 0.4, 0.2) * 4;
	sets[2] = circle(&p);

	// Fourth set:
	p = uv;
	p.Y += sin(5.*p.X)*sin(2.*p.Y + 1.4*time);
	p.X += sin(5.*p.Y)*sin(p.X) + sin(time);
	p.X = fmodf(p.X, 0.4) * 3;
	p.Y = fmodf(p.Y, 0.4) * 3;
	sets[3] = circle(&p);

	// Fifth set:
	p = uv;
	p.X += sin(2.*uv.Y)*sin(5.*uv.X + 2.5*time);
	p.Y += sin(2.*uv.X)*sin(5.*uv.X + 2.0*time);
	p.X = fmodf(p.X, 0.2) * 5;
	p.Y = fmodf(p.Y, 0.2) * 5;
	sets[4] = circle(&p);

	Colours[0] = (uint8_t)((sets[0] + sets[2] + 0.2 * sets[4]) * 255);
	Colours[1] = (uint8_t)((sets[0] + sets[1] + 0.5 * sets[3] + 0.4 * sets[4]) * 255);
	Colours[2] = (uint8_t)((sets[1] + sets[2] + 0.7 * sets[4]) * 255);
	Colours[3] = (uint8_t)255;
}