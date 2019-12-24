#include "TileMap.h"

////////////////////////////////////////////////////////////
bool TileMap::load(const std::string& t_tilsetFileName,
	const sf::Vector2u& t_tileSize,
	const std::vector<unsigned> &t_tiles,
	unsigned t_width,
	unsigned t_height)
{
	if (!m_tileset.loadFromFile(t_tilsetFileName))
		return false;

	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(t_width * t_height * 4);


	for (unsigned x{ 0 }; x < t_width; x++)
		for (unsigned y{ 0 }; y < t_height; y++) {

			unsigned tileNum{ t_tiles[y * t_width + x] };

			// Find position in tile texture
			unsigned tex_x{ tileNum % (m_tileset.getSize().x / t_tileSize.x) };
			unsigned tex_y{ tileNum / (m_tileset.getSize().x / t_tileSize.x) };

			auto quad{ &m_vertices[(x + y * t_width) * 4] };

			// Define 4 corners
			quad[0].position = sf::Vector2f(x * t_tileSize.x, y * t_tileSize.y);
			quad[1].position = sf::Vector2f((x + 1) * t_tileSize.x, y * t_tileSize.y);
			quad[2].position = sf::Vector2f((x + 1) * t_tileSize.x, (y + 1) * t_tileSize.y);
			quad[3].position = sf::Vector2f(x * t_tileSize.x, (y + 1) * t_tileSize.y);

			// Define 4 texture corners
			quad[0].texCoords = sf::Vector2f(tex_x * t_tileSize.x, tex_y * t_tileSize.y);
			quad[1].texCoords = sf::Vector2f((tex_x + 1) * t_tileSize.x, tex_y * t_tileSize.y);
			quad[2].texCoords = sf::Vector2f((tex_x + 1) * t_tileSize.x, (tex_y + 1) * t_tileSize.y);
			quad[3].texCoords = sf::Vector2f(tex_x * t_tileSize.x, (tex_y + 1) * t_tileSize.y);

		}
	return true;
}

////////////////////////////////////////////////////////////
void TileMap::draw(sf::RenderTarget& t_target, sf::RenderStates t_states)const {
	// Apply transformations
	t_states.transform *= getTransform();

	// Apply the tileset texture
	t_states.texture = &m_tileset;

	// Draw the vertex array
	t_target.draw(m_vertices, t_states);
}
