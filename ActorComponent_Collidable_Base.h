#ifndef ACTOR_COMPONENT_COLLIDABLE_BASE_H
#define ACTOR_COMPONENT_COLLIDABLE_BASE_H

#include <sstream>
#include <SFML/Graphics/Color.hpp>
#include "ActorComponent_Drawable.h"

struct SharedContext;

class ActorComponent_Collidable_Base : public ActorComponent_Base {
public:
	virtual void onCollision() = 0;
	static const sf::Color s_defaultOutlineColor;
	static const sf::Color s_defaultFillColor;
};

#endif // !ACTOR_COMPONENT_COLLIDABLE_BASE_H