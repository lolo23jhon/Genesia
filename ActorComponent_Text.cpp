#include <exception>
#include <iostream>
#include "ActorComponent_Text.h"
#include "ResourceHolder.h"
#include "Actor_Base.h"

////////////////////////////////////////////////////////////
ActorComponent_Text::ActorComponent_Text(SharedContext& t_context, const std::string& t_fontResourceName, const unsigned& t_size, const std::string& t_text) :
	ActorComponent_Drawable()
{

	Resource* resource{ t_context.m_resourceHolder->getResource(ResourceType::Font,t_fontResourceName) };
	if (!resource) {
		throw(std::runtime_error("ActorComponent_Text::ActorComponent_Text(SharedContext&, const std::string&, const unsigned&, const std::string&): The font resource was nullptr!"));
	}
	const auto& font{ std::get<sf::Font>(*resource) };
	m_text = std::move(sf::Text(t_text, font, t_size));
}

////////////////////////////////////////////////////////////
ActorComponent_Text::ActorComponent_Text(SharedContext& t_context, std::stringstream& t_stream) :
	ActorComponent_Drawable()
{

	std::string fontResName, size, text;
	if (!(t_stream >> fontResName >> size >> text)) {
		throw (std::runtime_error("ActorComponent_Text::ActorComponent_Text(SharedContext&, std::stringstream&): Could not read stream!"));
	}

	Resource* resource{ t_context.m_resourceHolder->getResource(ResourceType::Font,fontResName) };
	if (!resource) {
		throw(std::runtime_error("ActorComponent_Text::ActorComponent_Text(SharedContext&, const std::string&, const unsigned&, const std::string&): The font resource was nullptr!"));
	}
	const auto& font{ std::get<sf::Font>(*resource) };
	m_text = std::move(sf::Text(text, font, std::stoul(size)));

}

////////////////////////////////////////////////////////////
ActorComponent_Text::ActorComponent_Text(const sf::Text& t_text) :
	ActorComponent_Drawable(), m_text{ t_text } {}

////////////////////////////////////////////////////////////
void ActorComponent_Text::reset(Actor_Base* t_owner) {}

////////////////////////////////////////////////////////////
void ActorComponent_Text::draw(Actor_Base* t_owner) {
	t_owner->getContext().m_window->draw(m_text);
}

////////////////////////////////////////////////////////////
ActorComponentPtr ActorComponent_Text::clone(SharedContext& t_context)const {
	return std::make_unique<ActorComponent_Text>(m_text);
}

////////////////////////////////////////////////////////////
sf::Text& ActorComponent_Text::getText() { return m_text; }

////////////////////////////////////////////////////////////
const sf::Text& ActorComponent_Text::getText()const { return m_text; }

////////////////////////////////////////////////////////////
void ActorComponent_Text::update(Actor_Base* t_owner) {}
