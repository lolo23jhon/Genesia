#ifndef ACTOR_COMPONENT_SPRITE_H
#define ACTOR_COMPONENT_SPRITE_H

#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include "ActorComponent_Drawable.h"

class ResourceHolder;

class ActorComponent_Sprite : public ActorComponent_Drawable {
public:
	ActorComponent_Sprite(SharedContext& t_context, const std::string& t_texture, const sf::IntRect& t_spriteRect = sf::IntRect());
	ActorComponent_Sprite(SharedContext& t_context, std::stringstream& t_stream);
	const std::string& getTexture()const;
	bool setSprite(Actor_Base* t_owner, const std::string& t_texture, const sf::IntRect& t_spriteRect = sf::IntRect());
	const sf::IntRect& getTextureRect()const;
	void setTextureRext(const sf::IntRect& t_textureRect);
	void changeTextureRect(sf::IntRect t_spriteRect);

	void update(Actor_Base* t_owner); // Update any functionality
	void reset(Actor_Base* t_owner);	// General reset; optional
	void draw(Actor_Base* t_owner); // Draw on screen command

	ActorComponentPtr clone(SharedContext& t_context)const;

protected:

	std::string m_texture;
	sf::Sprite m_sprite;
	sf::IntRect m_textureRect;
	// <----- HERE: Decide the implementation of transformations throught the actor hierarchy
};

#endif // !SPRITE_H