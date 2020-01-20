#include "CollisionManager.h"
#include <algorithm>
#include "Engine.h"
#include "Organism.h"
#include "Food.h"
#include "MathHelpers.h"
#include "Collider.h"

////////////////////////////////////////////////////////////
static CollisionCallback bind(CollisionFunctor t_functor) { return std::bind(t_functor, std::placeholders::_1, std::placeholders::_2); }


////////////////////////////////////////////////////////////
CollisionManager::CollisionManager(Engine* t_owner, const sf::FloatRect& t_rootBounds) : m_engine{ t_owner }, m_root{ t_rootBounds,0U } {}

////////////////////////////////////////////////////////////
void CollisionManager::setBounds(const sf::FloatRect& t_bounds) { m_root.setBounds(t_bounds); }

// -------------------------------------------------------- COLLISION PAIRS IMPLEMENTATION	-----------------------------------------
////////////////////////////////////////////////////////////
static void CollisionFn_Organism_Food(Actor_Base* t_o, Actor_Base* t_f) { // The organism eats the food
	dynamic_cast<Organism*>(t_o)->eat(dynamic_cast<Food*>(t_f));
}


////////////////////////////////////////////////////////////
const CollisionSolver CollisionManager::s_collisions{
	{{ActorType::Organism, ActorType::Food}, bind(&CollisionFn_Organism_Food) }
};


////////////////////////////////////////////////////////////
bool CollisionManager::checkCollision(const Collider* t_obj1, const Collider* t_obj2) {
	// For the sake of simplicity, all colliders are assumed to be circles
	if (t_obj1->getOwner() == t_obj2->getOwner()) { return false; } // Same object
	return (mat::distance(t_obj1->getCenterPos(), t_obj2->getCenterPos()) < ((t_obj1->getAABB().width, t_obj2->getAABB().width) * 0.5f));
}


////////////////////////////////////////////////////////////
void CollisionManager::solveCollision(Collider* t_obj1, Collider* t_obj2) {
	auto it{ s_collisions.find(CollisionPair(t_obj1->getOwner()->getActorType(), t_obj2->getOwner()->getActorType())) };
	if (it == s_collisions.end()) { return; }
	it->second(t_obj1->getOwner(), t_obj2->getOwner());
}


////////////////////////////////////////////////////////////
void CollisionManager::update() {
	m_root.clear(); // Reset quad tree

	// Insert all the actors' colliders in to the machine
	m_engine->actorsForEach(
		[this](ActorPtr& t_actor) {	m_root.insert(&t_actor->getCollider());	}
	);

	Objects objects;
	m_engine->actorsForEach(
		[&objects, this](ActorPtr& t_actor) {
			objects.clear();
			Collider* obj1{ &t_actor->getCollider() };

			// Get the objects in potential of collision for each single actor
			m_root.getPotentialOverlaps(objects,obj1);

			for (auto& obj2 : objects) {
				if (checkCollision(obj1, obj2)) {
					solveCollision(obj1, obj2);
				}
			}
		}
	);
}

////////////////////////////////////////////////////////////
void CollisionManager::draw(sf::RenderWindow& t_window) {
	m_root.draw(t_window);
}