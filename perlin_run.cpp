//#include <unordered_set>
//#include <SFML/Graphics/Image.hpp>
//#include "PerlinNoise.h"
//#include "ColorMap.h"
//#include "file_io.h"
//
//
//int main() {
//	const char* fname{ "instructions.txt" };
//	auto f{ fio::readFile(fname) };
//	auto input{ f->begin() };
//
//
//	// Output files -------------------------------------------------------------------------------------------------------------------------------------------
//	const bool makePPM{ true };
//
//	// Noise generation proerties -----------------------------------------------------------------------------------------------------------------------------
//	unsigned w{ std::stoul(*++input) };
//	input++;
//	unsigned h{ std::stoul(*++input) };
//	input++;
//	const double n_scale{ std::stod(*++input) };
//	input++;
//	const unsigned n_octaves{ std::stoul(*++input) };
//	input++;
//	const double n_persistence{ std::stod(*++input) };
//	input++;
//	const double n_lacunarity{ std::stod(*++input) };
//
//	const bool useRandomSeed{ true };
//
//	const sf::Color water_c{ 14, 118, 168 };
//	const sf::Color sand_c{ 198, 166, 100 };
//	const sf::Color grass_c{ 86,125,70 };
//	const sf::Color mount_c{ 150, 159, 178 };
//	const sf::Color snow_c{ 255,255,255 };
//
//	const double water_h{ 128 };
//	const double sand_h{ 140 };
//	const double grass_h{ 195 };
//	const double mount_h{ 230 };
//	const double snow_h{ 255 };
//
//
//	// File names and paths for output of images --------------------------------------------------------------------------------------------------------------
//	const std::string pathPPM{ "C:/Users/sam's club/Desktop/NoiseTesting/PPM" };
//	std::string namePPM{ pathPPM + "/" +
//		"s" + std::to_string(n_scale) + "_" +
//		"o" + std::to_string(n_octaves) + "_" +
//		"p" + std::to_string(n_persistence) + "_" +
//		"l" + std::to_string(n_lacunarity) + "_" +
//	   std::to_string(w) + "x" + std::to_string(h) +
//	   ".ppm" };
//
//	// --------------------------------------------------------------------------------------------------------------------------------------------------------
//	// --------------------------------------------------------------------------------------------------------------------------------------------------------
//	if (useRandomSeed) {
//		Perlin::resetPermutationList();
//	}
//	auto h_map{ Perlin::noise2D(w, h, n_scale, n_octaves, n_persistence, n_lacunarity) };
//	h_map.mapValuesToRange(0.0, 255.0);
//	ColorMap c_map;
//	c_map.setNextColor(water_h, water_c)
//		.setNextColor(sand_h, sand_c)
//		.setNextColor(grass_h, grass_c)
//		.setNextColor(mount_h, mount_c)
//		.setNextColor(snow_h, snow_c);
//
//	fio::ImgData img;
//	img.reserve(w * h);
//	for (const auto& val : h_map) {
//		auto col{ c_map.getColor(val) };
//		img.emplace_back(col.r, col.g, col.b);
//	}
//
//	fio::writePPM(w, h, img, namePPM);
//
//	return 0;
//}