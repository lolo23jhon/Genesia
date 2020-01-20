#include "Quadtree.h"
#include "Collider.h"
#include "PreprocessorDirectves.h"
const unsigned Quadtree::S_MAX_LEVELS{ 5U };
const unsigned Quadtree::S_MAX_OBJECTS{ 5U };


////////////////////////////////////////////////////////////
Quadtree::Quadtree(const sf::FloatRect& t_bounds) : m_level{ 0U }, m_bounds{ t_bounds }{}

////////////////////////////////////////////////////////////
Quadtree::Quadtree(const sf::FloatRect& t_bounds, const unsigned& t_level) : m_level{ t_level }, m_bounds{ t_bounds } {}

////////////////////////////////////////////////////////////
const sf::FloatRect& Quadtree::getBounds()const { return m_bounds; }

////////////////////////////////////////////////////////////
void Quadtree::insert(Collider* t_obj) {

	if (m_nodes[0] != nullptr) {
		int index{ getIndex(t_obj->getAABB()) };
		if (index != THIS_TREE) {
			m_nodes[index]->insert(t_obj);
			return;
		}
	}

	m_objects.push_back(t_obj);

	if (m_objects.size() > S_MAX_OBJECTS&& m_level < S_MAX_LEVELS) {
		if (m_nodes[0] == nullptr) {
			split();
		}

		for (auto it{ m_objects.begin() }; it != m_objects.end();) {
			auto obj{ *it };
			int index{ getIndex(obj->getAABB()) };
			if (index != THIS_TREE) {
				m_nodes[index]->insert(obj);
				it = m_objects.erase(it);
			}
			else { it++; }
		}
	}
}

////////////////////////////////////////////////////////////
void Quadtree::clear() {
	m_objects.clear();
	for (unsigned i{ 0U }; i < 4U; i++) {
		if (m_nodes[i] != nullptr) {
			m_nodes[i]->clear();
			m_nodes[i] = nullptr;
		}
	}
}

////////////////////////////////////////////////////////////
void Quadtree::getPotentialOverlaps(Objects& t_out_objects, const Collider* t_obj)const {
	getPotentialOverlaps(t_out_objects, t_obj->getAABB());
}

void Quadtree::getPotentialOverlaps(Objects& t_out_objects, const sf::FloatRect& t_aabb)const {
	int index{ getIndex(t_aabb) };
	if (index != THIS_TREE && m_nodes[0] != nullptr) {
		m_nodes[index]->getPotentialOverlaps(t_out_objects, t_aabb);
	}

	t_out_objects.insert(t_out_objects.end(), m_objects.begin(), m_objects.end());
}

////////////////////////////////////////////////////////////
void Quadtree::setBounds(const sf::FloatRect& t_bounds) { m_bounds = t_bounds; }


////////////////////////////////////////////////////////////
int Quadtree::getIndex(const sf::FloatRect& t_aabbObj)const {
	int index{ -1 };
	float vertMidPnt{ m_bounds.left + m_bounds.width * 0.5f };
	float horiMidPnt{ m_bounds.top + m_bounds.height * 0.5f };

	bool up{ t_aabbObj.top < horiMidPnt && (t_aabbObj.height + t_aabbObj.top < horiMidPnt) };
	bool down{ t_aabbObj.top > horiMidPnt };
	bool left(t_aabbObj.left < vertMidPnt && (t_aabbObj.left + t_aabbObj.width < vertMidPnt));
	bool right{ t_aabbObj.left > vertMidPnt };

	if (right) {
		if (up) { index = CHILD_NE; }
		else if (down) { index = CHILD_SE; };
	}
	else if (left) {
		if (up) { index = CHILD_NW; }
		else if (down) { index = CHILD_SW; }
	}

	return index;
}


////////////////////////////////////////////////////////////
void Quadtree::split() {

	float w{ m_bounds.width * 0.5f };
	float h{ m_bounds.height * 0.5f };

	m_nodes[CHILD_NE] = std::make_unique<Quadtree>(sf::FloatRect(m_bounds.left + w, m_bounds.top, w, h), m_level + 1);
	m_nodes[CHILD_NW] = std::make_unique<Quadtree>(sf::FloatRect(m_bounds.left, m_bounds.top, w, h), m_level + 1);
	m_nodes[CHILD_SW] = std::make_unique<Quadtree>(sf::FloatRect(m_bounds.left, m_bounds.top + h, w, h), m_level + 1);
	m_nodes[CHILD_SE] = std::make_unique<Quadtree>(sf::FloatRect(m_bounds.left + w, m_bounds.top + h, w, h), m_level + 1);
}


#if defined(_DEBUG) &&  IS_DRAW_COLLISION_QUADTREE == 1
#include <SFML/Graphics/RectangleShape.hpp>

static const sf::RectangleShape S_RECT_SHAPE{ []() {
	sf::RectangleShape rect;
	rect.setFillColor({ 0, 0, 255, 255 });
	rect.setOutlineColor({ 0,0,0, 255 });
	rect.setOutlineThickness(2.f);
	return std::move(rect);
}() };

#endif // defined(_DEBUG) &&  IS_DRAW_COLLISION_QUADTREE == 1


////////////////////////////////////////////////////////////
void Quadtree::draw(sf::RenderWindow& t_window) {
#if defined(_DEBUG) &&  IS_DRAW_COLLISION_QUADTREE == 1
	
	// Have children? Tell THEM to draw
	if (m_nodes[0] != nullptr) {
		for (auto& n : m_nodes) {
			n->draw(t_window);
		}
	}
	// Don't have children? Then you draw yourself
	else {
		auto rect{ S_RECT_SHAPE };
		rect.setFillColor({0,0,255, static_cast<sf::Uint8>((255 * m_level)/S_MAX_LEVELS ) });
		rect.setSize({ m_bounds.width, m_bounds.height });
		rect.setPosition(m_bounds.left, m_bounds.top);
		t_window.draw(rect);
	}
#endif // defined(_DEBUG) &&  IS_DRAW_COLLISION_QUADTREE == 1
}
