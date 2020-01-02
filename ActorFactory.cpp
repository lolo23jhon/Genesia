#include "ActorFactory.h"
#include "Actor_Base.h"

////////////////////////////////////////////////////////////
ActorFactory::ActorFactory() : m_actor_wip{ nullptr } {}

////////////////////////////////////////////////////////////
std::unique_ptr<Actor_Base> ActorFactory::create() {
	return std::move(m_actor_wip);
}


////////////////////////////////////////////////////////////
ActorFactory& ActorFactory::newActor() {
	m_actor_wip = std::make_unique<Actor_Base>();
	return *this;
}

////////////////////////////////////////////////////////////
ActorFactory& ActorFactory::setPosition(const sf::Vector2f& t_position) {
	m_actor_wip->setPosition(t_position.x, t_position.y);
	return *this;
}

////////////////////////////////////////////////////////////
ActorFactory& ActorFactory::setRotation(const float& t_rotationDeg) {
	m_actor_wip->setRotation(t_rotationDeg);
}

////////////////////////////////////////////////////////////
template <class TComponent, class ...TArgs>ActorFactory& ActorFactory::addComponent(TArgs...  t_compArgs) {
	m_actor_wip->forceInsertComponent(t_componentType, std::make_unique<TComponent>(m_actor_wip.get(), t_compArgs...));
	return *this;
}

////////////////////////////////////////////////////////////
ActorFactory& ActorFactory::abortConstruction() { 
	m_actor_wip.reset(); 
	return *this;
}
