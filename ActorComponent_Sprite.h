#ifndef ACTOR_COMPONENT_SPRITE_H
#define ACTOR_COMPONENT_SPRITE_H

#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include "ActorComponent.h"

class ActorComponent_Sprite : public ActorComponent_Drawable {

protected:

	sf::Sprite m_sprite;

	ActorComponent_Sprite(Actor_Base* t_owner, const std::string& t_spriteIdentifier);

	void update(Actor_Base* t_owner); // Update any functionality
	void reset(Actor_Base* t_owner);	// General reset; optional
	void draw(Actor_Base* t_owner); // Draw on screen command

   // <----- HERE: Decide the implementation of transformations throught the actor hierarchy

};

#endif // !SPRITE_H