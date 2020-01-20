#ifndef QUADTREE_H
#define QUADTREE_H

#include <memory>
#include <vector>
#include <array>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Quadtree;
class Collider;

using Nodes = std::array<std::unique_ptr<Quadtree>, 4U>;
using Objects = std::vector<Collider*>;



class Quadtree {
	enum {
		THIS_TREE = -1,
		CHILD_NE,
		CHILD_NW,
		CHILD_SW,
		CHILD_SE
	};

	static const unsigned S_MAX_OBJECTS;
	static const unsigned S_MAX_LEVELS;

	Nodes m_nodes;
	Objects m_objects;
	unsigned m_level;
	sf::FloatRect m_bounds;
public:
	Quadtree(const sf::FloatRect& t_bounds); // Root constructor
	Quadtree(const sf::FloatRect& t_bounds, const unsigned& t_level); // Child node constructor
	const sf::FloatRect& getBounds()const;
	void insert(Collider* t_obj);
	void clear();
	void getPotentialOverlaps(Objects& t_out_objects, const Collider* t_obj)const;
	void getPotentialOverlaps(Objects& t_out_objects, const sf::FloatRect& t_aabb)const;
	void setBounds(const sf::FloatRect& t_bounds);

	void draw(sf::RenderWindow& t_window);


private:
	int getIndex(const sf::FloatRect& t_aabbObj)const;
	void split();

};

#endif // !QUADTREE_H
