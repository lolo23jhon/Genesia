#include "Quadtree.h"

Collidable::Collidable(const sf::FloatRect& t_bound, Actor_Base* t_owner) : m_bound{ t_bound } {}


void QuadTree::subdivide() {
	float w{ m_bounds.width * 0.5f };
	float h{ m_bounds.height * 0.5f };
	float x{ 0.f };
	float y{ 0.f };

	for (unsigned i{ 0U }; i < 4U; i++) {
		switch (i) {
		case 0U: x = m_bounds.left * w;		y = m_bounds.top;		break; // Top right
		case 1U: x = m_bounds.left;			y = m_bounds.top;		break; // Top left
		case 2U: x = m_bounds.left;			y = m_bounds.top * h;	break; // Bottom left
		case 3U: x = m_bounds.left * w;		y = m_bounds.top * h;	break; // Bottom right
		}
		m_children[i] = std::make_unique<QuadTree>({ x, y, w, h }, m_capacity, m_maxLevel);
		m_children[i]->m_level = m_level + 1;
		m_children[i]->m_parent = this;
	}
	m_isLeaf = false;
}

void QuadTree::discardEmptyBuckets() {
	if (!m_objects.empty()) { return; }
	if (!m_isLeaf) {
		for (const auto& child : m_children) {
			if (!child->m_isLeaf || !child->m_objects.empty()) { return; }
		}
	}
	if (clear(), !m_parent) {
		m_parent->discardEmptyBuckets();
	}
}

QuadTree* QuadTree::getLeaf(const sf::FloatRect& t_bound) {
	if (!m_isLeaf) {
		if (QuadTree * child{ getChild(m_bounds) }) {
			return child->getLeaf(m_bounds);
		}
	}
	return this;
}

QuadTree* QuadTree::getChild(const sf::FloatRect& t_bound)const {
	bool left{m_bounds.left + m_bounds.width < m_};
}
bool QuadTree::insert(Collidable* t_collidable);
bool QuadTree::remove(Collidable* t_collidable);
bool QuadTree::update(Collidable* t_collidable);
void QuadTree::draw(SharedContext& t_context);
void QuadTree::clear();
unsigned QuadTree::totalChildren()const;
unsigned QuadTree::totalObjects()const;
std::vector<Collidable*>& getObjectsInBound_unchecked(const sf::FloatRect& t_bound);
QuadTree::QuadTree(const sf::FloatRect& t_bound, const unsigned& t_capacity, const unsigned& t_maxLevel);
QuadTree::QuadTree(const QuadTree& t_rhs);
QuadTree::QuadTree();