#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <functional>
#include <unordered_map>
#include <SFML/Graphics/RenderWindow.hpp>
#include "QuadTree.h"
#include "unordered_pair_hash.hpp"

class Engine;
class Actor_Base;
enum class ColliderType;
struct PairHash;

using CollisionFunctor = void(*)(Actor_Base*, Actor_Base*);
using CollisionCallback = std::function<void(Actor_Base*, Actor_Base*)>; // Every combination of collidable actor types get a collision solving callback
using CollisionPair = std::pair<ColliderType, ColliderType>; // Used for its relational operator. Meant to serve as keys in the collision solver hashmap.
using CollisionSolver = std::unordered_map<CollisionPair, CollisionCallback, std::hash<std::pair<ColliderType, ColliderType>>, utilities::UnorderedEqual>;

enum class ColliderType {
	Organism,
	Food
};

struct Collider {
	sf::FloatRect m_aabb; // Axis aligned bounding box used for quadtree collision narrowing, not resolution of interpenetration
	ColliderType m_type;
	Actor_Base* m_owner;
	float m_radius;
	float m_x;
	float m_y;

	void update(const ColliderType& y_type, Actor_Base* t_owner, const float& t_x, const float t_y, const float& t_w, const float& t_h, const float& t_radius);
	const float& get_x()const;
	const float& get_y()const;
	float getWidth()const;
	float getHeight()const;


};


class CollisionManager {

	QuadTree m_root;
	Engine* m_engine;

	static const CollisionSolver s_collisions;
	CollisionManager(const CollisionManager& t_rhs) = delete;

public:
	CollisionManager(Engine* t_owner);
	void setQuadTreeBounds(const sf::FloatRect& t_bounds);
	bool checkCollision(const Collider& t_obj1, const Collider& t_obj2);
	void solveCollision(Collider& t_obj1, Collider& t_obj2);
	void update();
	void draw(sf::RenderWindow& t_window);

};

#endif // !COLLISION_MANAGER_H