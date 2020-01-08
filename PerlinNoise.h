#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <cstdint>

class PerlinNoise {


	static uint8_t hash(int32_t t_i);
	static uint8_t s_perm[256];

public:
	static float noise(float t_x);
	static float noise(float t_x, float t_y);
	static float noise(float t_x, float t_y, float t_z);

	static void resetPermutationList(unsigned t_seed);

};

#endif // !PERLIN_NOISE_H