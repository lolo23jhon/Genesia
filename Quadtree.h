#ifndef QUADTREE_H
#define QUADTREE_H

#include <array>
#include <memory>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class QuadTree;
class Actor_Base;
struct SharedContext;

using ActorId = unsigned;
using QuadTreePtr = std::unique_ptr<QuadTree>;

struct Collidable {
public:
	sf::FloatRect m_bound;

	Collidable(const sf::FloatRect& t_bound, Actor_Base* t_owner);
private:
	QuadTree* m_quadTree;
	Collidable(const Collidable& t_rhs) = delete;
};


class QuadTree {

	bool m_isLeaf;
	unsigned m_level;
	unsigned m_capacity;
	unsigned m_maxLevel;
	sf::FloatRect m_bounds;
	QuadTree* m_parent;
	std::array<QuadTreePtr, 4U> m_children;
	sf::Text m_text;
	sf::RectangleShape m_shape;
	std::vector<Collidable*> m_objects;
	std::vector<Collidable*> m_foundObjects;

	void subdivide();
	void discardEmptyBuckets();
	QuadTree* getChild(const sf::FloatRect& t_bound)const;

public:
	QuadTree(const sf::FloatRect& t_bound, const unsigned& t_capacity, const unsigned& t_maxLevel);
	QuadTree(const QuadTree& t_rhs);
	QuadTree();
	
	bool insert(Collidable *t_collidable);
	bool remove(Collidable* t_collidable);
	bool update(Collidable* t_collidable);
	std::vector<Collidable*>& getObjectsInBound_unchecked(const sf::FloatRect& t_bound);
	unsigned totalChildren()const;
	unsigned totalObjects()const;
	void draw(SharedContext& t_context);
	void clear();
	QuadTree* getLeaf(const sf::FloatRect& t_bound);
};

#endif // !QUADTREE_H