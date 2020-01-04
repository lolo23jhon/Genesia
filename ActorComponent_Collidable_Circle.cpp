#include <exception>
#include <memory>
#include "ActorComponent_Collidable_Circle.h"
#include "Actor_Base.h"

////////////////////////////////////////////////////////////
ActorComponent_Collidable_Circle::ActorComponent_Collidable_Circle(SharedContext& t_context, const float& t_radius) :
	ActorComponent_Drawable(), m_circle{ t_radius }{
	m_circle.setOutlineColor(s_defaultOutlineColor);
	m_circle.setFillColor(s_defaultFillColor);
}

////////////////////////////////////////////////////////////
ActorComponent_Collidable_Circle::ActorComponent_Collidable_Circle(SharedContext& t_context, std::stringstream& t_stream) : ActorComponent_Drawable(), m_circle{} {
	std::string radius;
	if (!(t_stream >> radius)) {
		throw(std::runtime_error("ActorComponent_Collidable_Circle::ActorComponent_Collidable_Circle(SharedContext&, std::stringstream&): Cannot read stream!"));
	}
	m_circle.setRadius(std::stof(radius));
	m_circle.setOutlineColor(s_defaultOutlineColor);
	m_circle.setFillColor(s_defaultFillColor);
}

////////////////////////////////////////////////////////////
ActorComponent_Collidable_Circle::ActorComponent_Collidable_Circle(const sf::CircleShape& t_circle) : m_circle{ t_circle } {}

////////////////////////////////////////////////////////////
const sf::CircleShape& ActorComponent_Collidable_Circle::getCircle()const { return m_circle; }

////////////////////////////////////////////////////////////
sf::CircleShape& ActorComponent_Collidable_Circle::getCircle() { return m_circle; }

////////////////////////////////////////////////////////////
void ActorComponent_Collidable_Circle::update(Actor_Base* t_owner) {
	ActorComponent_Drawable::update(t_owner);
	m_circle.setPosition(t_owner->getPosition());
}

////////////////////////////////////////////////////////////
void ActorComponent_Collidable_Circle::reset(Actor_Base* t_owner) {}

////////////////////////////////////////////////////////////
void ActorComponent_Collidable_Circle::draw(Actor_Base* t_owner) {
	t_owner->getContext().m_window->draw(m_circle);
}

////////////////////////////////////////////////////////////
ActorComponentPtr ActorComponent_Collidable_Circle::clone(SharedContext& t_context)const {
	//return std::make_unique<ActorComponent_Collidable_Circle>(m_circle);
	return nullptr;
}

////////////////////////////////////////////////////////////
void ActorComponent_Collidable_Circle::onCollision() {}