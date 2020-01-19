#include "DebugObject.h"
#if defined(_DEBUG) && IS_DEBUG_OBJECTS == 1
#include <cassert>

static const sf::Font& font() {
	static bool first{ true };
	static sf::Font font;
	if (first) {
		bool res{ font.loadFromFile("C:\\Users\\sam's club\\source\\repos\\CreatureObervatory\\resources\\Font_consola.ttf") };
		assert(res && "Quadtree: Could not load default font for debug display.");
	}
	return font;
}

unsigned DebugObject::s_num{ 0U };

DebugObject::DebugObject(const sf::Vector2f& t_position) :m_position{ t_position }, m_shape{ sf::RectangleShape() }, m_text{ sf::Text() }, m_id{s_num}{
	s_num++;
	m_text.setFont(font());
	m_text.setFillColor({ 255, 255, 255, 255 });
	m_text.setCharacterSize(10U);
	m_shape.setSize({100.f,100.f});
	m_shape.setOrigin({0.5f,0.5f});
	m_shape.setFillColor({ 255,255,255,255 });
	m_shape.setOutlineColor({ 0,255,255 });
	m_shape.setOutlineThickness(0.5f);
}

DebugObject::~DebugObject() { s_num--; }

void DebugObject::draw(sf::RenderWindow& t_window) {
	m_text.setPosition(m_position);
	m_shape.setPosition(m_position);
	t_window.draw(m_shape);
	t_window.draw(m_text);
}

#endif // defined(_DEBUG) && IS_DEBUG_OBJECTS == 1