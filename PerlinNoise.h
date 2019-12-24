#ifndef PERLIN_NOUSE_2D_H
#define PERLIN_NOISE_2D_H

#include <memory>
#include <random>
#include <ctime>
#include <algorithm>
#include <vector>
#include "HeightMap.h"

class Perlin {

	static std::vector<uint8_t> s_permutation;


	Perlin() = delete;
	static double fade(double t_t);
	static double lerp(double t_t, double t_a, double t_b);
	static double grad(int t_hash, double t_x, double t_y, double t_z);

public:
	static double noise(double t_x, double t_y = 0.0, double t_z = 0.0);
	static std::unique_ptr<HeightMap> noise2D(unsigned t_w, unsigned t_h, double t_scale, unsigned t_octaves, double t_persistence = 0.5, double t_lacunarity = 2.0);
	static void resetPermutationList(unsigned t_seed = std::time(nullptr));
	static double clip(double t_n, double t_min, double t_max);
	static double mapRange(double t_min, double t_max, double t_newMin, double t_newMax, double t_n);

};

#endif