#include "PreprocessorDirectves.h"

#if defined(_DEBUG) && IS_DEBUG_OBJECTS == 1

#ifndef DEBUG_OBJECT_H
#define DEBUG_OBJECT_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

struct DebugObject {

	const unsigned m_id;
	sf::Text m_text;
	sf::RectangleShape m_shape;
	sf::Vector2f m_position;
	
	DebugObject(const sf::Vector2f& t_position);
	~DebugObject();
	void draw(sf::RenderWindow& t_window);
private:
	static unsigned s_num;

};

#endif // ! DEBUG_OBJECT_H

#endif // defined(_DEBUG) && IS_DEBUG_OBJECTS == 1