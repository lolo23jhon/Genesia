//#include "TileMap.h"
//#include "PerlinNoise.h"
//
//int main() {
//
//	const unsigned tile_w{ 1 };
//	const unsigned tile_h{ 1 };
//
//	const unsigned win_w{ 1800 };
//	const unsigned win_h{ 1000 };
//
//	const unsigned map_w{ win_w / tile_w };
//	const unsigned map_h{ win_h / tile_h };
//
//	const double water_h{ 110 };
//	const double sand_h{ 140 };
//	const double grass_h{ 195 };
//	const double mount_h{ 230 };
//	const double snow_h{ 255 };
//	// ---------------------------------------------------------------------------
//	// ---------------------------------------------------------------------------
//
//
//	Perlin::resetPermutationList();
//	auto height_map{ Perlin::noise2D(map_w,map_h,200,1) };
//	height_map->mapValuesToRange(0.0, 255.0);
//	std::vector<unsigned> tile_map;
//	tile_map.reserve(map_w * map_h);
//	for (const auto& h : *height_map.get()) {
//
//		unsigned tile_id{ 0 };
//		if (h <= water_h) {
//			tile_id = 0;
//		}
//		else if (h > water_h&& h <= sand_h) {
//			tile_id = 1;
//		}
//		else if (h > sand_h&& h <= grass_h) {
//			tile_id = 2;
//		}
//		else if (h > grass_h&& h <= mount_h) {
//			tile_id = 3;
//		}
//		else {
//			tile_id = 4;
//		}
//		tile_map.emplace_back(tile_id);
//	}
//
//
//	sf::RenderWindow window(sf::VideoMode(win_w, win_h), "Tilemap test");
//
//	TileMap map;
//
//	if (!map.load("tileset_1x1.png", sf::Vector2u(tile_w, tile_h), tile_map, map_w, map_h))
//		return 1;
//
//	while (window.isOpen()) {
//
//		sf::Event e;
//		while (window.pollEvent(e)) {
//			if (e.type == sf::Event::Closed)
//				window.close();
//		}
//
//
//		window.clear();
//		window.draw(map);
//		window.display();
//	}
//	return 0;
//}