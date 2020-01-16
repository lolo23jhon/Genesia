#include "Quadtree.h"
#include "CollisionManager.h"
#include <cassert>

const unsigned QuadTree::S_MAX_LEVELS{ 5U };
const unsigned QuadTree::S_MAX_OBJECTS{ 10U };

////////////////////////////////////////////////////////////
QuadTree::QuadTree(const unsigned& t_level, const sf::FloatRect& t_bounds) :
	m_level{ t_level }, m_bounds{ t_bounds }, m_objects{}, m_nodes{}
{}

////////////////////////////////////////////////////////////
void QuadTree::clear() {
	m_objects.clear();
	for (unsigned i{ 0U }; i < 4U; i++) {
		if (m_nodes[i]) {
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
int QuadTree::getIndex(const Collider* t_obj) {
	int index{ -1 };
	// Midpoints
	float vert_mp{ m_bounds.left + (m_bounds.width / 2.f) };
	float hori_mp{ m_bounds.top + (m_bounds.height / 2.f) };


	bool top_quad{ t_obj->get_x() < hori_mp && t_obj->get_y() + t_obj->getHeight() < hori_mp }; // Fit on top quadrant
	bool bot_quad{ t_obj->get_x() > hori_mp }; // Fit on bottom quadrant

	// Object fits on left side
	if (t_obj->get_x() < vert_mp && t_obj->get_x() + t_obj->getWidth() < vert_mp) {
		if (top_quad) { index = 1; }
		else if (bot_quad) { index = 2; }
	}

	// Object fits on right side
	else if (t_obj->get_x() > vert_mp) {
		if (top_quad) { index = 0; }
		else if (bot_quad) { index = 3; }
	}

	return static_cast<unsigned>(index);
}

////////////////////////////////////////////////////////////
void QuadTree::insert(const Collider* t_obj) {
	if (m_nodes[0]) {
		int index{ getIndex(t_obj) };
		if (index != -1) {
			m_nodes[index]->insert(t_obj);
			return;
		}
	}

	m_objects.emplace_back(t_obj);
	if (m_objects.size() > S_MAX_OBJECTS&& m_level < S_MAX_LEVELS) {
		if (!m_nodes[0]) { subdivide(); }


		for (unsigned i{ 0U }; i < m_objects.size();) {
			int index{ getIndex(m_objects.at(i)) };
			if (index != -1) {
				auto it{ m_objects.begin() + i };
				Collider* ptr{ *it };
				m_objects.erase(it);
				m_nodes[index]->insert(ptr);
			}
			else { i++; }
		}
	}
}

////////////////////////////////////////////////////////////
void QuadTree::retrieve(Objects& t_out_objects, const Collider* t_obj) {
	int index{ getIndex(t_obj) };
	if (index != -1 && m_nodes[0]) {
		m_nodes[index]->retrieve(t_out_objects, t_obj);
	}
	t_out_objects.insert(t_out_objects.end(), m_objects.cbegin(), m_objects.cend());
}