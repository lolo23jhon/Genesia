#include "CollisionManager.h"
#include <algorithm>
#include "Engine.h"
#include "Organism.h"
#include "Food.h"
#include "MathHelpers.h"
#include "DebugObject.h"

////////////////////////////////////////////////////////////
static CollisionCallback bind(CollisionFunctor t_functor) { return std::bind(t_functor, std::placeholders::_1, std::placeholders::_2); }


////////////////////////////////////////////////////////////
static void CollisionFn_Organism_Food(Actor_Base* t_o, Actor_Base* t_f) { // The organism eats the food
	dynamic_cast<Organism*>(t_o)->eat(dynamic_cast<Food*>(t_f));
}


////////////////////////////////////////////////////////////
CollisionManager::CollisionManager(Engine* t_owner) : m_engine{ t_owner }, m_root{ 0U, sf::FloatRect() } {}

////////////////////////////////////////////////////////////
const CollisionSolver CollisionManager::s_collisions{
	{{ColliderType::Organism, ColliderType::Food}, bind(&CollisionFn_Organism_Food) }
};

////////////////////////////////////////////////////////////
void CollisionManager::setQuadTreeBounds(const sf::FloatRect& t_bounds) { m_root.setBounds(t_bounds); }

////////////////////////////////////////////////////////////
void Collider::update(const ColliderType& t_type, Actor_Base* t_owner, const float& t_x, const float t_y, const float& t_w, const float& t_h, const float& t_radius) {
	m_type = t_type;
	m_owner = t_owner;
	m_x = t_x;
	m_y = t_y;
	m_radius = t_radius;
	m_aabb = sf::FloatRect(m_x - t_w * 0.5f, m_y - t_h * 0.5f, t_w, t_h); // Transform from center-based x and y to top left corner
}


////////////////////////////////////////////////////////////
const float& Collider::get_x()const { return m_x; }


////////////////////////////////////////////////////////////
const float& Collider::get_y()const { return m_y; }


////////////////////////////////////////////////////////////
float Collider::getWidth()const { return m_aabb.width; }


////////////////////////////////////////////////////////////
float Collider::getHeight()const { return m_aabb.height; }


////////////////////////////////////////////////////////////
bool CollisionManager::checkCollision(const Collider& t_obj1, const Collider& t_obj2) {
	float dist{ mat::distance(t_obj1.m_x, t_obj1.m_y, t_obj2.m_x, t_obj2.m_y) };
	return  dist < (t_obj1.m_radius + t_obj2.m_radius);
}


////////////////////////////////////////////////////////////
void CollisionManager::solveCollision(Collider& t_obj1, Collider& t_obj2) {
	auto it{ s_collisions.find(CollisionPair(t_obj1.m_type, t_obj2.m_type)) };
	if (it == s_collisions.end()) { return; }
	it->second(t_obj1.m_owner, t_obj2.m_owner);
}


////////////////////////////////////////////////////////////
void CollisionManager::update() {
	m_root.clear(); // Reset quad tree

	// Insert all the actors' colliders in to the machine
	m_engine->actorsForEach(
		[this](ActorPtr& t_actor) {	m_root.insert(t_actor->getCollider());	}
	);

	Objects objects;
	m_engine->actorsForEach(
		[&objects, this](ActorPtr& t_actor) {
			objects.clear();
			Collider& obj1{ t_actor->getCollider() };

			// Get the objects in potential of collision for each single actor
			m_root.retrieve(objects, obj1);

			for (auto& obj2 : objects) {
				if (checkCollision(obj1, *obj2)) {
					solveCollision(obj1, *obj2);
				}
			}
		}
	);
}

////////////////////////////////////////////////////////////
void CollisionManager::draw(sf::RenderWindow& t_window) {
	m_root.draw(t_window);
}