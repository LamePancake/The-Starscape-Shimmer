#ifndef PROCEDURAL_FUNC_H
#define PROCEDURAL_FUNC_H

#include <cstdint>

typedef struct _Vec2 {
	double X;
	double Y;
} Vec2;

typedef struct _Vec3 {
	double X;
	double Y;
	double Z;
} Vec3;

typedef struct _Vec4 {
	double X;
	double Y;
	double Z;
	double W;
} Vec4;

typedef void (*GeneratorFunc)(uint8_t* Colours, Vec2* Pos, Vec2* Bounds, double ElapsedTime);

#define step(x, y) ((y) < (x) ? 0.0 : 1.0)

// Procedural generation functions that can be used in the actor
void LSD(uint8_t* Colours, Vec2* Pos, Vec2* Bounds, double ElapsedTime);
void Plasma(uint8_t* Colours, Vec2* Pos, Vec2* Bounds, double ElapsedTime);
void CirclePlasma(uint8_t* Colours, Vec2* Pos, Vec2* Bounds, double ElapsedTime);

#endif