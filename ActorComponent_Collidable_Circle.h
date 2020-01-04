#ifndef ACTOR_COMPONENT_COLLIDABLE_CIRCLE_H
#define ACTOR_COMPONENT_COLLIDABLE_CIRCLE_H

#include <sstream>
#include <SFML/Graphics/CircleShape.hpp>
#include "Collidable_Base.h"
#include "ActorComponent_Drawable.h"
struct SharedContext;

class ActorComponent_Collidable_Circle : public Collidable_Base, public ActorComponent_Drawable {

	sf::CircleShape m_circle;

public:
	ActorComponent_Collidable_Circle(SharedContext& t_context, const float& t_radius);
	ActorComponent_Collidable_Circle(SharedContext& t_context, std::stringstream& t_stream);
	ActorComponent_Collidable_Circle(const sf::CircleShape& t_circle);
	const sf::CircleShape& getCircle()const;
	sf::CircleShape& getCircle();

	void update(Actor_Base* t_owner);
	void reset(Actor_Base* t_owner);
	void draw(Actor_Base* t_owner);

	ActorComponentPtr clone(SharedContext& t_context)const;

	void onCollision(); // Empty body, just a shape, let a child class implement actual hitbox

};
#endif // !ACTOR_COMPONENT_COLLIDABLE_CIRCLE_H