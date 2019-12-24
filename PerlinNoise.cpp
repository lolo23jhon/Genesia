#include "PerlinNoise.h"




////////////////////////////////////////////////////////////
std::vector<uint8_t> Perlin::s_permutation{
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
	130,185,37,217,173,250,115,119,192,110,45,215,226,137,162,42,
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
double Perlin::fade(double t_t) {
	return t_t * t_t * t_t * (t_t * (t_t * 6.0 - 15.0) + 10.0);
}


////////////////////////////////////////////////////////////
double Perlin::lerp(double t_t, double t_a, double t_b) {
	return t_a + t_t * (t_b - t_a);
}

////////////////////////////////////////////////////////////
double Perlin::grad(int t_hash, double t_x, double t_y, double t_z) {
	int h{ t_hash & 15 };
	double u{ h < 8 ? t_x : t_y };
	double v{ h < 4 ? t_y : h == 12 || h == 14 ? t_x : t_z };
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}


////////////////////////////////////////////////////////////
void Perlin::resetPermutationList(unsigned t_seed) {
	std::mt19937 rng{ t_seed };
	std::uniform_int_distribution<unsigned> dist{ 0,255 };

	for (unsigned i{ 0 }; i < 256u; i++) {
		unsigned val{ dist(rng) };
		s_permutation[i] = val;
		s_permutation[i + 256u] = val;
	}
}


////////////////////////////////////////////////////////////
double Perlin::noise(double t_x, double t_y, double t_z) {
	int X{ (int)floor(t_x) & 255 };
	int Y{ (int)floor(t_y) & 255 };
	int Z{ (int)floor(t_z) & 255 };

	t_x -= floor(t_x);
	t_y -= floor(t_y);
	t_z -= floor(t_z);

	double u{ fade(t_x) };
	double v{ fade(t_y) };
	double w{ fade(t_z) };

	const auto& p{ s_permutation };

	int A{ p[X] + Y };
	int AA{ p[A] + Z };
	int AB{ p[A + 1] + Z };
	int B{ p[X + 1] + Y };
	int BA{ p[B] + Z };
	int BB{ p[B + 1] + Z };

	return  lerp(w,
		lerp(v,
			lerp(u,
				grad(p[AA], t_x, t_y, t_z),
				grad(p[BA], t_x - 1, t_y, t_z)),
			lerp(u,
				grad(p[AB], t_x, t_y - 1, t_z),
				grad(p[BB], t_x - 1, t_y - 1, t_z))),
		lerp(v,
			lerp(u,
				grad(p[AA + 1], t_x, t_y, t_z - 1),
				grad(p[BA + 1], t_x - 1, t_y, t_z - 1)),
			lerp(u,
				grad(p[AB + 1], t_x, t_y - 1, t_z - 1),
				grad(p[BB + 1], t_x - 1, t_y - 1, t_z - 1))));
}



////////////////////////////////////////////////////////////
std::unique_ptr<HeightMap> Perlin::noise2D(unsigned t_w, unsigned t_h, double t_scale, unsigned t_octaves, double t_persistence, double t_lacunarity) {
	if (!t_scale) {
		t_scale += 0.000001;
	}

	std::vector<double> noise_vec;
	noise_vec.reserve(t_w * t_h);

	double min{ INFINITY };
	double max{ -INFINITY };

	for (unsigned y{ 0 }; y < t_h; y++) {
		for (unsigned x{ 0 }; x < t_w; x++) {

			double amplitude{ 1.0 };
			double frequency{ 1.0 };
			double  noiseHeight{ 0.0 };



			for (unsigned i{ 0 }; i < t_octaves; i++) {
				double nx{ frequency * (double)x / t_scale };
				double ny{ frequency * (double)y / t_scale };

				double val{ noise(nx,ny) };
				noiseHeight += val * amplitude;

				amplitude *= t_persistence;
				frequency *= t_lacunarity;
			}

			if (noiseHeight < min) {
				min = noiseHeight;
			}
			else if (noiseHeight > max) {
				max = noiseHeight;
			}
			noise_vec.emplace_back(noiseHeight);
		}
	}

	for (auto& val : noise_vec) {
		val = Perlin::mapRange(min, max, 0.0, 1.0, val);
	}

	return  std::make_unique<HeightMap>(t_w, t_h, noise_vec);

}


////////////////////////////////////////////////////////////
double Perlin::clip(double t_n, double t_min, double t_max) {
	return std::max(t_min, std::min(t_n, t_max));

}


////////////////////////////////////////////////////////////
double Perlin::mapRange(double t_min, double t_max, double t_newMin, double t_newMax, double t_n) {
	return t_newMin + (t_n - t_min) * (t_newMax - t_newMin) / (t_max - t_min);
}
