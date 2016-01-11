#include "TheStarscapeShimmer.h"
#include <cstdint>
#include <cmath>
#include "ProceduralFunc.h"

void Plasma(uint8_t* Colours, Vec2* Pos, Vec2* Bounds, double ElapsedTime) {
	double ShiftedX = (Pos->X / Bounds->X * 2) - 1.0;
	double ShiftedY = (Pos->Y / Bounds->Y * 2) - 1.0;

	float mov0 = ShiftedX + ShiftedY + cosf(sinf(ElapsedTime) * 2.0) * 100.f + sinf(ShiftedX / 100.f) * 1000.f;
	float mov1 = ShiftedX / 0.9 + ElapsedTime;
	float mov2 = ShiftedX / 0.2;
	float c1 = fabs(sinf(mov1 + ElapsedTime) / 2.f + mov2 / 2.f - mov1 - mov2 + ElapsedTime);
	float c2 = fabs(sinf(c1 + sinf(mov0 / 1000.f + ElapsedTime) + sinf(ShiftedY / 40.f + ElapsedTime) + sinf((ShiftedX + ShiftedY) / 100.f) * 3.f));
	float c3 = fabs(sinf(c2 + cosf(mov1 + mov2 + c2) + cosf(mov2) + sinf(ShiftedX / 1000.f)));

	Colours[0] = (uint8_t)(c1 * 255);
	Colours[1] = (uint8_t)(c2 * 255);
	Colours[2] = (uint8_t)(c3 * 255);
	Colours[4] = (uint8_t)255;
}