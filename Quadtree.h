#ifndef QUADTREE_H
#define QUADTREE_H

#include <array>
#include <memory>
#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

struct Collider;
class QuadTree;


using Objects = std::vector<Collider*>; // Non-data-owning ptrs
using Nodes = std::array<std::unique_ptr<QuadTree>, 4U>;

class QuadTree {
	static const unsigned S_MAX_LEVELS;
	static const unsigned S_MAX_OBJECTS;

	unsigned m_level;
	Objects m_objects;
	sf::FloatRect m_bounds;
	Nodes m_nodes;

public:
	QuadTree(const unsigned& t_level, const sf::FloatRect& t_bounds);
	void setBounds(const sf::FloatRect& t_bounds);
	void clear();
	void subdivide();
	int getIndex(const Collider& t_obj);
	void insert(Collider& t_obj); // Makes non-owning ptr of the object
	void retrieve(Objects& t_out_objs, const Collider& t_obj); // Retrieves all the objects that could collide with the objcext

	unsigned i{ 0U };
	void draw(sf::RenderTarget& t_window);
};

#endif // !QUADTREE_H
