#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <functional>
#include <unordered_map>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Quadtree.h"
#include "unordered_pair_hash.hpp"

class Engine;
class Actor_Base;
enum class ActorType;
struct PairHash;

using CollisionFunctor = void(*)(Actor_Base*, Actor_Base*);
using CollisionCallback = std::function<void(Actor_Base*, Actor_Base*)>; // Every combination of collidable actor types get a collision solving callback
using CollisionPair = std::pair<ActorType, ActorType>; // Used for its relational operator. Meant to serve as keys in the collision solver hashmap.
using CollisionSolver = std::unordered_map<CollisionPair, CollisionCallback, std::hash<CollisionPair>, utilities::UnorderedEqual>;



class CollisionManager {

	Quadtree m_root;
	Engine* m_engine;

	static const CollisionSolver s_collisions;
	CollisionManager(const CollisionManager& t_rhs) = delete;

public:
	CollisionManager(Engine* t_owner, const sf::FloatRect& t_rootBounds);
	void setBounds(const sf::FloatRect& t_bounds);
	bool checkCollision(const Collider* t_obj1, const Collider* t_obj2);
	void solveCollision(Collider* t_obj1, Collider* t_obj2);
	void update();
	void draw(sf::RenderWindow& t_window);

};

#endif // !COLLISION_MANAGER_H