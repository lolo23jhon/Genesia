#ifndef ACTOR_COMPONENET_TEXT_H
#define ACTOR_COMPONENET_TEXT_H

#include <sstream>
#include <string>
#include "ActorComponent_Drawable.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SharedContext.h"


class ActorComponent_Text : public ActorComponent_Drawable {
	sf::Text m_text;
public:
	ActorComponent_Text(const sf::Text& t_text);
	ActorComponent_Text(SharedContext& t_context, const std::string& t_fontResourceName,  const unsigned& t_size, const std::string& t_text);
	ActorComponent_Text(SharedContext& t_context, std::stringstream& t_stream);

	sf::Text& getText();
	const sf::Text& getText()const;

	void update(Actor_Base* t_owner); // Update any functionality
	void reset(Actor_Base* t_owner); // General reset; optional
	void draw(Actor_Base* t_owner); // Draw on screen command

	ActorComponentPtr clone(SharedContext& t_context)const; // Makes a copy of the actor component and hands it out as a unique ptr

};

#endif // !ACTOR_COMPONENET_TEXT_H