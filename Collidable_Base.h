#ifndef COLLIDABLE_BASE_H
#define COLLIDABLE_BASE_H

#include <sstream>
#include <SFML/Graphics/Color.hpp>
#include "ActorComponent_Drawable.h"

struct SharedContext;

class Collidable_Base {
public:
	virtual void onCollision() = 0;
	virtual bool isColliding(Actor_Base* t_owner, Collidable_Base* t_collider) = 0;
	static void solveCollision(Actor_Base* t_owner1, Collidable_Base* t_collider1, Actor_Base* t_owner2, Collidable_Base* t_collider2);
	static const sf::Color s_defaultOutlineColor;
	static const sf::Color s_defaultFillColor;
};

#endif // !COLLIDABLE_BASE_H