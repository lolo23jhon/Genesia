#ifndef QUADTREE_H
#define QUADTREE_H

#include <array>
#include <memory>
#include <vector>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

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
	void draw(sf::RenderWindow& t_window);

private:
	void getDrawablesFromChildren(std::vector<sf::RectangleShape>& t_rects, std::vector<sf::Text>& t_tags, std::vector<sf::RectangleShape>& t_aabbs)const;
};

#endif // !QUADTREE_H
