#include "PerlinNoise.h"
#include <random>
#include <cmath>

////////////////////////////////////////////////////////////
static int32_t fastfloor(float t_fp) {
	int32_t i{ static_cast<int32_t>(t_fp) };
	return (t_fp < i) ? (i - 1) : (i);
}

////////////////////////////////////////////////////////////
static float grad(int32_t t_hash, float t_x) {
	const int32_t h{ t_hash & 0x0F };
	float grad = 1.0f + (h & 7);
	if ((h & 8) != 0) grad = -grad;
	return (grad * t_x);
}

////////////////////////////////////////////////////////////
static float grad(int32_t t_hash, float t_x, float t_y) {
	const int32_t h{ t_hash & 0x3F };
	const float u{ h < 4 ? t_x : t_y };
	const float v{ h < 4 ? t_y : t_x };
	return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

////////////////////////////////////////////////////////////
static float grad(int32_t t_hash, float t_x, float t_y, float t_z) {
	int h{ t_hash & 15 };
	float u{ h < 8 ? t_x : t_y };
	float v{ h < 4 ? t_y : h == 12 || h == 14 ? t_x : t_z };
	return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

////////////////////////////////////////////////////////////
uint8_t PerlinNoise::hash(int32_t t_i) {
	return s_perm[static_cast<uint8_t>(t_i)];
}


////////////////////////////////////////////////////////////
uint8_t PerlinNoise::s_perm[256]{
34,125,70,85,187,178,104,64,227,187,87,91,160,252,173,6,
19,19,6,222,125,158,98,189,39,13,67,101,55,200,96,174,
197,226,183,166,131,247,18,116,2,224,20,55,20,35,3,155,
200,246,174,214,89,181,151,112,254,148,114,244,31,48,185,220,
212,68,123,235,72,22,76,95,109,42,127,111,71,62,186,162,
20,245,239,160,191,165,153,121,69,200,49,14,201,173,212,94,
143,44,18,28,114,239,237,85,226,229,18,202,196,237,173,209,
180,117,105,9,62,65,139,44,136,45,147,88,191,194,248,167,
247,222,232,110,69,6,150,211,162,244,32,121,86,114,95,216,
32,128,241,205,11,65,50,221,93,17,148,45,116,197,207,65,
238,137,73,111,194,23,193,120,255,41,54,70,113,4,124,185,
71,200,136,80,6,22,18,226,109,43,139,57,140,161,236,21,
39,36,61,228,26,18,86,106,241,55,31,240,15,69,152,254,
20,170,90,78,69,62,32,223,167,52,194,240,254,179,221,235,
60,183,62,27,214,144,151,197,1,99,224,188,219,23,112,112,
130,185,37,217,173,250,115,119,192,110,45,215,226,137,162,42
};


////////////////////////////////////////////////////////////
void PerlinNoise::resetPermutationList(unsigned t_seed) {
	std::mt19937 engine{ t_seed };
	std::uniform_int_distribution<int> rng{ 0,255 };
	for (unsigned i{ 0U }; i < 256U; i++) {
		s_perm[i] = rng(engine);
	}
}

////////////////////////////////////////////////////////////
float PerlinNoise::noise(float t_x) {
	float n0, n1;
	int32_t i0{ fastfloor(t_x) };
	int32_t i1{ i0 + 1 };
	float x0{ t_x - i0 };
	float x1{ x0 - 1.0f };
	float t0{ 1.0f - x0 * x0 };
	t0 *= t0;
	n0 = t0 * t0 * grad(hash(i0), x0);
	float t1{ 1.0f - x1 * x1 };
	t1 *= t1;
	n1 = t1 * t1 * grad(hash(i1), x1);

	return 0.395f * (n0 + n1);
}


////////////////////////////////////////////////////////////
float PerlinNoise::noise(float t_x, float t_y) {
	float n0, n1, n2;

	static const float F2 = 0.366025403f;
	static const float G2 = 0.211324865f;

	const float s{ (t_x + t_y) * F2 };
	const float xs{ t_x + s };
	const float ys{ t_y + s };
	const int32_t i{ fastfloor(xs) };
	const int32_t j{ fastfloor(ys) };

	const float t{ static_cast<float>(i + j)* G2 };
	const float X0{ i - t };
	const float Y0{ j - t };
	const float x0{ t_x - X0 };
	const float y0{ t_y - Y0 };

	int32_t i1, j1;
	if (x0 > y0) {
		i1 = 1;
		j1 = 0;
	}
	else {
		i1 = 0;
		j1 = 1;
	}

	const float x1{ x0 - i1 + G2 };
	const float y1{ y0 - j1 + G2 };
	const float x2{ x0 - 1.0f + 2.0f * G2 };
	const float y2{ y0 - 1.0f + 2.0f * G2 };

	const int gi0{ hash(i + hash(j)) };
	const int gi1{ hash(i + i1 + hash(j + j1)) };
	const int gi2{ hash(i + 1 + hash(j + 1)) };

	float t0{ 0.5f - x0 * x0 - y0 * y0 };
	if (t0 < 0.0f) {
		n0 = 0.0f;
	}
	else {
		t0 *= t0;
		n0 = t0 * t0 * grad(gi0, x0, y0);
	}

	float t1{ 0.5f - x1 * x1 - y1 * y1 };
	if (t1 < 0.0f) {
		n1 = 0.0f;
	}
	else {
		t1 *= t1;
		n1 = t1 * t1 * grad(gi1, x1, y1);
	}

	float t2{ 0.5f - x2 * x2 - y2 * y2 };
	if (t2 < 0.0f) {
		n2 = 0.0f;
	}
	else {
		t2 *= t2;
		n2 = t2 * t2 * grad(gi2, x2, y2);
	}

	return 45.23065f * (n0 + n1 + n2);
}


////////////////////////////////////////////////////////////
float PerlinNoise::noise(float t_x, float t_y, float t_z) {
	float n0, n1, n2, n3; 

	static const float F3 = 1.0f / 3.0f;
	static const float G3 = 1.0f / 6.0f;

	float s{ (t_x + t_y + t_z) * F3 };
	int i { fastfloor(t_x + s)};
	int j { fastfloor(t_y + s)};
	int k { fastfloor(t_z + s)};
	float t{ (i + j + k) * G3 };
	float X0 { i - t}; 
	float Y0 { j - t};
	float Z0 { k - t};
	float x0 { t_x - X0}; 
	float y0 { t_y - Y0};
	float z0 { t_z - Z0};

	int i1, j1, k1; 
	int i2, j2, k2; 
	if (x0 >= y0) {
		if (y0 >= z0) {
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0; 
		}
		else if (x0 >= z0) {
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; 
		}
		else {
			i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; 
		}
	}
	else {
		if (y0 < z0) {
			i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; 
		}
		else if (x0 < z0) {
			i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1;
		}
		else {
			i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		}
	}

	float x1 {x0 - i1 + G3};
	float y1 {y0 - j1 + G3};
	float z1 {z0 - k1 + G3};
	float x2 {x0 - i2 + 2.0f * G3}; 
	float y2 {y0 - j2 + 2.0f * G3};
	float z2 {z0 - k2 + 2.0f * G3};
	float x3 {x0 - 1.0f + 3.0f * G3};
	float y3 {y0 - 1.0f + 3.0f * G3};
	float z3 {z0 - 1.0f + 3.0f * G3};

	int gi0{ hash(i + hash(j + hash(k))) };
	int gi1{ hash(i + i1 + hash(j + j1 + hash(k + k1))) };
	int gi2{ hash(i + i2 + hash(j + j2 + hash(k + k2))) };
	int gi3{ hash(i + 1 + hash(j + 1 + hash(k + 1))) };

	float t0{ 0.6f - x0 * x0 - y0 * y0 - z0 * z0 };
	if (t0 < 0) {
		n0 = 0.0;
	}
	else {
		t0 *= t0;
		n0 = t0 * t0 * grad(gi0, x0, y0, z0);
	}
	float t1{ 0.6f - x1 * x1 - y1 * y1 - z1 * z1 };
	if (t1 < 0) {
		n1 = 0.0;
	}
	else {
		t1 *= t1;
		n1 = t1 * t1 * grad(gi1, x1, y1, z1);
	}
	float t2{ 0.6f - x2 * x2 - y2 * y2 - z2 * z2 };
	if (t2 < 0) {
		n2 = 0.0;
	}
	else {
		t2 *= t2;
		n2 = t2 * t2 * grad(gi2, x2, y2, z2);
	}
	float t3{ 0.6f - x3 * x3 - y3 * y3 - z3 * z3 };
	if (t3 < 0) {
		n3 = 0.0;
	}
	else {
		t3 *= t3;
		n3 = t3 * t3 * grad(gi3, x3, y3, z3);
	}

	return 32.0f * (n0 + n1 + n2 + n3);
}
