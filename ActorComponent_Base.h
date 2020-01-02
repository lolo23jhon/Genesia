#ifndef ACTOR_COMPONENT_BASE_H
#define ACTOR_COMPONENT_BASE_H

#include <SFML/System/Vector2.hpp>

class Actor_Base;

enum class ActorComponentType {
	Box,
	Ai,
	Sprite,
	Text
};


class ActorComponent_Base {
public:
	virtual void update(Actor_Base* t_owner) = 0; // Update any functionality
	virtual void reset(Actor_Base* t_owner)	 = 0; // General reset; optional
	virtual void draw(Actor_Base* t_owner)   = 0; // Draw on screen command

	static bool isComponentDrawable(const ActorComponentType& t_componentId);

};

#endif // !ACTOR_COMPONENT_BASE_H