#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <string>
#include <SFML/Graphics.hpp>

class TileMap : public sf::Drawable, public sf::Transformable {

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;

public:
	bool load(const std::string& t_tilsetFileName,
		const sf::Vector2u& t_tileSize,
		const std::vector<unsigned>& t_tiles,
		unsigned t_width,
		unsigned t_height);

	virtual void draw(sf::RenderTarget& t_target, sf::RenderStates t_states)const;

};

#endif