#include <cassert>
#include "Quadtree.h"
#include "CollisionManager.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "ResourceHolder.h"
#include "PreprocessorDirectves.h"

const unsigned QuadTree::S_MAX_LEVELS{ 5U };
const unsigned QuadTree::S_MAX_OBJECTS{ 10U };
static const sf::Color S_FILL_COLOR{ 255, 255, 255, 20 }; 
static const sf::Color S_BORDER_COLOR{ 0,0,255,255 }; // Solid blue
static const unsigned S_TEXT_SIZE{ 10U };

#if defined(_DEBUG) &&  IS_DRAW_COLLISION_QUADTREE == 1 
static const sf::Font& font() {
	static bool first{ true };
	static sf::Font font;
	if (first) {
		bool res{ font.loadFromFile("C:\\Users\\sam's club\\source\\repos\\CreatureObervatory\\resources\\Font_consola.ttf") };
		assert(res && "Quadtree: Could not load default font for debug display.");
	}
	return font;
}
#endif // defined(_DEBUG) &&  IS_DRAW_COLLISION_QUADTREE == 1 

////////////////////////////////////////////////////////////
QuadTree::QuadTree(const unsigned& t_level, const sf::FloatRect& t_bounds) :
	m_level{ t_level }, m_bounds{ t_bounds }, m_objects{}, m_nodes{ nullptr,nullptr ,nullptr ,nullptr }
{}

////////////////////////////////////////////////////////////
void QuadTree::setBounds(const sf::FloatRect& t_bounds) { m_bounds = t_bounds; }

////////////////////////////////////////////////////////////
void QuadTree::clear() {
	m_objects.clear();
	if (m_level < S_MAX_LEVELS) {
		for (unsigned i{ 0U }; i < 4U; i++) {
			m_nodes[i].reset();
		}
	}
}

////////////////////////////////////////////////////////////
void QuadTree::subdivide() {
	float sub_w{ m_bounds.width / 2.f };
	float sub_h{ m_bounds.height / 2.f };
	float x{ m_bounds.left };
	float y{ m_bounds.top };

	// (In SFML +y is down)
	m_nodes[0] = std::make_unique<QuadTree>(m_level + 1, sf::FloatRect(x + sub_w, y, sub_w, sub_h)); // Top right corner
	m_nodes[1] = std::make_unique<QuadTree>(m_level + 1, sf::FloatRect(x, y, sub_w, sub_h)); // Top left corner
	m_nodes[2] = std::make_unique<QuadTree>(m_level + 1, sf::FloatRect(x + sub_w, y + sub_h, sub_w, sub_h)); // Bottom right corner
	m_nodes[3] = std::make_unique<QuadTree>(m_level + 1, sf::FloatRect(x, y + sub_h, sub_w, sub_h)); // Bottom left corner
}

////////////////////////////////////////////////////////////
int QuadTree::getIndex(const Collider& t_obj) {
	int index{ -1 };

	// Midpoints
	float vert_mp{ m_bounds.left + (m_bounds.width / 2.f) };
	float hori_mp{ m_bounds.top + (m_bounds.height / 2.f) };

	bool top_quad{ t_obj.get_x() < hori_mp && t_obj.get_y() + t_obj.getHeight() < hori_mp }; // Fit on top quadrant
	bool bot_quad{ t_obj.get_x() > hori_mp }; // Fit on bottom quadrant

	// Object fits on left side
	if (t_obj.get_x() < vert_mp && t_obj.get_x() + t_obj.getWidth() < vert_mp) {
		if (top_quad) { index = 1; }
		else if (bot_quad) { index = 2; }
	}

	// Object fits on right side
	else if (t_obj.get_x() > vert_mp) {
		if (top_quad) { index = 0; }
		else if (bot_quad) { index = 3; }
	}

	return index;
}

////////////////////////////////////////////////////////////
void QuadTree::insert(Collider& t_obj) {
	// Dos the node have children?
	if (m_nodes[0]) {
		int index{ getIndex(t_obj) };

		// Does the object fit in a child?
		if (index != -1) {
			m_nodes[index]->insert(t_obj);
			return;
		}
	}
	m_objects.push_back(&t_obj);
	// After adding, determine if the present node should be split
	if (m_objects.size() > S_MAX_OBJECTS&& m_level < S_MAX_LEVELS) {
		if (!m_nodes[0]) { subdivide(); } // Create children nodes

		for (auto it{ m_objects.begin() }; it != m_objects.end();) {
			int index{ getIndex(**it) };
			if (index != -1) {
				// Hand the object to child
				Collider* obj{ *it };
				assert(obj->m_owner && "QuadTree::insert(Collider&): Collider component owner is nullptr!");
				it = m_objects.erase(it);
				m_nodes[index]->insert(*obj);
			}
			else { it++; }
		}
	}
}

////////////////////////////////////////////////////////////
void QuadTree::retrieve(Objects& t_out_objects, const Collider& t_obj) {
	int index{ getIndex(t_obj) };
	// If it fits here, check if a child can take it
	if (index != -1 && m_nodes[0]) {
		m_nodes[index]->retrieve(t_out_objects, t_obj);
	}
	t_out_objects.insert(t_out_objects.end(), m_objects.cbegin(), m_objects.cend());
}

////////////////////////////////////////////////////////////
void QuadTree::draw(sf::RenderTarget& t_window) {
#if defined(_DEBUG) &&  IS_DRAW_COLLISION_QUADTREE == 1 

	// Draw the children nodes (if any)
	if (m_level < S_MAX_LEVELS && !m_nodes.empty()) {
		for (const auto& n : m_nodes) { if (n) { n->draw(t_window); } }
	}
 	sf::Text text;
	text.setFont(font());
	text.setString(std::string(m_level,' ') + std::to_string(m_level));
	text.setPosition(m_bounds.left, m_bounds.top);
	text.setFillColor(S_BORDER_COLOR);
	text.setOutlineColor(S_BORDER_COLOR);
	text.setCharacterSize(S_TEXT_SIZE);
	sf::RectangleShape rectangle({ m_bounds.width, m_bounds.height });
	rectangle.setFillColor(S_FILL_COLOR); // Transparent green fill
	rectangle.setOutlineColor(S_BORDER_COLOR); // Blue borders
	rectangle.setOutlineThickness(2.f);
	rectangle.setPosition({ m_bounds.left, m_bounds.top });
	t_window.draw(rectangle);
	t_window.draw(text);

#endif // defined(_DEBUG) &&  IS_DRAW_COLLISION_QUADTREE == 1 
}