#include "TheStarscapeShimmer.h"
#include <cstdint>
#include <cmath>
#include "ProceduralFunc.h"
#include "TrigLookup.h"
#define PI 3.14159

#define FAST_TRIG

#ifdef FAST_TRIG

#define sin_func(x) SinLookup((x))
#define cos_func(x) CosLookup((x))

#else

#define sin_func(x) sin((x))
#define cos_func(x) cos((x))

#endif

void Random2(Vec2* In, Vec2* Out) {
	double j = 4906.0 * sin_func(In->X * 169.7 + In->Y * 5.8);
	double j1 = 512.0 * j;
	double j2 = 0.125 * 512.0 * j;
	Vec2 r = {j1 - (uint64_t)j1, j2 - (uint64_t)j2};
	Out->X = r.X - 0.5;
	Out->Y = r.Y - 0.5;
}

const float F2 = 0.3660254;
const float G2 = -0.2113249;

float simplex2d(Vec2 *p){
	Vec2 s = {floor(p->X + (p->X + p->Y)*F2), floor(p->Y + (p->X + p->Y)*F2)};
	Vec2 x = {p->X - s.X - (s.X + s.Y)*G2, p->Y - s.Y - (s.X + s.Y)*G2};
	float e = (x.X - x.Y) < 0.0 ? 0.0 : 1.0;
	Vec2 i1 = Vec2{e, 1.0 - e};
	Vec2 x1 = {x.X - i1.X - G2, x.Y - i1.Y - G2};
	Vec2 x2 = {x.X - 1.0 - 2.0*G2, x.Y - 1.0 - 2.0 * G2};
	Vec3 w, d;
	w.X = fmax(0.5 - ((x.X * x.X) + (x.Y * x.Y)), 0.0);
	w.Y = fmax(0.5 - ((x1.X * x1.X) + (x1.Y * x1.Y)), 0.0);
	w.Z = fmax(0.5 - ((x2.X * x2.X) + (x2.Y * x2.Y)), 0.0);

	Vec2 randVec = s;
	Random2(&randVec, &randVec);
	d.X = randVec.X * x.X + randVec.Y * x.Y;
	
	randVec.X = s.X + i1.X;
	randVec.Y = s.Y + i1.Y;
	Random2(&randVec, &randVec);
	d.Y = randVec.X * x1.X + randVec.Y * x1.Y;
	
	randVec.X = s.X + 1.0;
	randVec.Y = s.Y + 1.0;
	Random2(&randVec, &randVec);
	d.Z = randVec.X * x2.X + randVec.Y * x2.Y;
	
	w.X = w.X * w.X * w.X;
	w.Y = w.Y * w.Y * w.Y;
	w.Z = w.Z * w.Z * w.Z;

	d.X *= w.X;
	d.Y *= w.Y;
	d.Z *= w.Z;

	return (d.X * 70.0) + (d.Y * 70.0) + (d.Z * 70.0);
}

void rgb2yiq(Vec3* Color, Vec3* Out){
	/*
	  mat3(0.299,  0.587,  0.114,
	       0.596, -0.274, -0.321,
		   0.211, -0.523,  0.311);
	*/

	Out->X = 0.299 * Color->X + 0.587 * Color->Y + 0.114 * Color->Z;
	Out->Y = 0.596 * Color->X + -0.274 * Color->Y + -0.321 * Color->Z;
	Out->Z = 0.211 * Color->X + -0.523 * Color->Y + 0.311 * Color->Z;
}

void yiq2rgb(Vec3* Color, Vec3* Out){
	/*
	  mat3(1.,  0.956,  0.621,
	       1., -0.272, -0.647,
		   1., -1.107, 1.705)
	*/
	Out->X = Color->X + 0.956 * Color->Y + 0.621 * Color->Z;
	Out->Y = Color->X + -0.272 * Color->Y + -0.647 * Color->Z;
	Out->Z = Color->X + -1.107 * Color->Y + 1.705 * Color->Z;
}

void convertRGB443quant(Vec3* Color, Vec3* Out){
	Out->X = fmod(Color->X, 1.0 / 16);
	Out->Y = fmod(Color->Y, 1.0 / 16);
	Out->Z = fmod(Color->Z, 1.0 / 8);
}

void convertRGB443(Vec3* Color, Vec3* Out){
	convertRGB443quant(Color, Out);
	Out->X = Color->X - Out->X;
	Out->Y = Color->Y - Out->Y;
	Out->Z = Color->Z - Out->Z;
}

void sin_funccos_func(float x, Vec2* Out){
	Out->X = sin_func(x);
	Out->Y = cos_func(x);
}

void Rotate2d(Vec2* UV, float phi, Vec2* Out){
	Vec2 t;
	sin_funccos_func(phi, &t);
	Out->X = UV->X * t.Y - UV->Y * t.X;
	Out->Y = UV->X * t.X + UV->Y * t.Y;
}

void Rotate3d(Vec3* p, Vec3* v, float phi, Vec3* Out){
	double LenV = sqrt(v->X * v->X + v->Y * v->Y + v->Z * v->Z);
	v->X = v->X / LenV;
	v->Y = v->Y / LenV;
	v->Z = v->Z / LenV;
	Vec2 t;
	sin_funccos_func(-phi, &t);

	double s = t.X;
	double c = t.Y;
	double x = -v->X;
	double y = -v->Y;
	double z = -v->Z;

	/*
	mat4 M = mat4(x*x*(1. - c) + c,   x*y*(1. - c) - z*s, x*z*(1. - c) + y*s, 0.,
		          y*x*(1. - c) + z*s, y*y*(1. - c) + c,   y*z*(1. - c) - x*s, 0.,
				  z*x*(1. - c) - y*s, z*y*(1. - c) + x*s, z*z*(1. - c) + c,   0.,
				  0.,                 0.,                 0.,                 1.);
	*/

	double oneSubC = 1 - c;
	double xzOneSubC = x*z*oneSubC;
	double xyOneSubC = x*y*oneSubC;
	double yzOneSubC = y*z*oneSubC;
	double xS = x*s;
	double yS = y*s;
	double zS = z*s;

	// Manually do the multiplication
	// Too bad I don't know SIMD instructions, huh?
	Vec4 Rot = {p->X, p->Y, p->Z, 1.0};
	Out->X = (Rot.X * (x * x * oneSubC + c)) + (Rot.Y * (xyOneSubC - zS)) + (Rot.Z * (xzOneSubC + yS));
	Out->Y = (Rot.X * (xyOneSubC + zS)) + (Rot.Y * (y * y * oneSubC + c)) + (Rot.Z * (yzOneSubC - xS));
	Out->Z = (Rot.X * (xzOneSubC - yS)) + (Rot.Y * (yzOneSubC + xS)) + (Rot.Z * (z * z * oneSubC + c));
}

double varazslat(Vec2* position, double time){
	double color = 0.0;
	double t = 2.0 *time;
	color += sin_func(position->X*cos_func(t / 10.0)*20.0) + cos_func(position->X*cos_func(t / 15.)*10.0);
	color += sin_func(position->Y*sin_func(t / 5.0)*15.0) + cos_func(position->X*sin_func(t / 25.)*20.0);
	color += sin_func(position->X*sin_func(t / 10.0)*  .2) + sin_func(position->Y*sin_func(t / 35.)*10.);
	color *= sin_func(t / 10.)*.5;

	return color;
}

// Stolen from the following ShaderToy shader: https://www.shadertoy.com/view/ldBSRd
void LSD(uint8_t* Colours, Vec2* Pos, Vec2* Bounds, double ElapsedTime)
{
	Vec2 UV = { ((Pos->X / Bounds->X) - 0.5) * 2, ((Pos->Y / Bounds->Y) - 0.5) * 2 };
	Vec3 vlsd = {0, 1, 0};
	Vec3 Rot = {1.0, 1.0, 0.0};
	Rotate3d(&vlsd, &Rot, ElapsedTime, &vlsd);
	Rotate3d(&vlsd, &Rot, ElapsedTime, &vlsd);
	Rotate3d(&vlsd, &Rot, ElapsedTime, &vlsd);

	Vec2 changedUV = {UV.X * 0.917, UV.Y * 0.917};
	double simplexResult;
	Vec2 sin_funccos_funcVal;
	sin_funccos_func(.3457 * ElapsedTime + .3423, &sin_funccos_funcVal);
	sin_funccos_funcVal.X *= 0.75;
	sin_funccos_funcVal.Y *= 0.75;
	simplexResult = simplex2d(&changedUV);
	Vec2 v0 = { sin_funccos_funcVal.X - simplexResult, sin_funccos_funcVal.Y - simplexResult };

	changedUV = {UV.X * .521, UV.Y * .521};
	sin_funccos_func(.7435 * ElapsedTime + .4565, &sin_funccos_funcVal);
	sin_funccos_funcVal.X *= 0.75;
	sin_funccos_funcVal.Y *= 0.75;
	simplexResult = simplex2d(&changedUV);
	Vec2 v1 = { sin_funccos_funcVal.X - simplexResult, sin_funccos_funcVal.Y - simplexResult };

	changedUV = {UV.X * .521, UV.Y * .759};
	sin_funccos_func(.5345 * ElapsedTime + .3434, &sin_funccos_funcVal);
	sin_funccos_funcVal.X *= 0.75;
	sin_funccos_funcVal.Y *= 0.75;
	simplexResult = simplex2d(&changedUV);
	Vec2 v2 = { sin_funccos_funcVal.X - simplexResult, sin_funccos_funcVal.Y - simplexResult };

	Vec3 color = {(UV.X - v0.X) * vlsd.X + (UV.Y - v0.Y) * vlsd.Y, 
		          (UV.X - v1.X) * vlsd.Y + (UV.Y - v1.Y) * vlsd.Z, 
		          (UV.X - v2.X) * vlsd.Z + (UV.Y - v2.Y) * vlsd.X};

	Vec2 UVV0 = {UV.X + v0.X, UV.Y + v0.Y};
	Vec2 UVV1 = {UV.X + v1.X, UV.Y + v1.Y};
	Vec2 UVV2 = {UV.X + v2.X, UV.Y + v2.Y};

	Vec2 temp = UVV0;
	double colourRMul = (16 * simplex2d(&temp));
	
	temp.X *= 2;
	temp.Y *= 2;
	colourRMul += 8 * simplex2d(&temp); 
	
	temp.X *= 2;
	temp.Y *= 2;
	colourRMul += 4 * simplex2d(&temp);
	
	temp.X *= 2;
	temp.Y *= 2;
	colourRMul += 2 * simplex2d(&temp);

	temp.X *= 2;
	temp.Y *= 2;
	colourRMul += simplex2d(&temp);
	colourRMul /= 32;

	temp = UVV1;
	double colourGMul = (16 * simplex2d(&temp));
	temp.X *= 2;
	temp.Y *= 2;
	colourGMul += 8 * simplex2d(&temp); 

	temp.X *= 2;
	temp.Y *= 2;
	colourGMul += 4 * simplex2d(&temp);

	temp.X *= 2;
	temp.Y *= 2;
	colourGMul += 2 * simplex2d(&temp);

	temp.X *= 2;
	temp.Y *= 2;
	colourGMul += simplex2d(&temp);
	colourGMul /= 32;

	temp = UVV2;
	double colourBMul = (16 * simplex2d(&temp));
	temp.X *= 2;
	temp.Y *= 2;
	colourBMul += 8 * simplex2d(&temp); 

	temp.X *= 2;
	temp.Y *= 2;
	colourBMul += 4 * simplex2d(&temp);

	temp.X *= 2;
	temp.Y *= 2;
	colourBMul += 2 * simplex2d(&temp);

	temp.X *= 2;
	temp.Y *= 2;
	colourBMul += simplex2d(&temp);
	colourBMul /= 32;

	color.X *= 0.2 + 2.5 * colourRMul;
	color.Y *= 0.2 + 2.5 * colourGMul;
	color.Z *= 0.2 + 2.5 * colourBMul;

	yiq2rgb(&color, &color);

	Colours[0] = (uint8_t)(pow(color.X, 0.45) * 255);
	Colours[1] = (uint8_t)(pow(color.Y, 0.45) * 255);
	Colours[2] = (uint8_t)(pow(color.Z, 0.45) * 255);
	Colours[3] = (uint8_t)255;
}
