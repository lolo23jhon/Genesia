#include "ActorComponent_Sprite.h"
#include "Actor_Base.h"
#include "MathHelpers.hpp"
#include "ResourceHolder.hpp"


////////////////////////////////////////////////////////////
ActorComponent_Sprite::ActorComponent_Sprite(Actor_Base* t_owner, const std::string& t_spriteIdentifier) : ActorComponent_Drawable(){
	
	m_sprite =
	t_owner->getContext().m_resourceHolder.
}


////////////////////////////////////////////////////////////
void ActorComponent_Sprite::update(Actor_Base* t_owner) {
	m_sprite.setPosition(t_owner->getPosition() + m_positionOffsetFromOwner);
	m_sprite.setRotation(mat::normalizeAngle(t_owner->getRotation() + m_angularOffsetFromOwner));
}

////////////////////////////////////////////////////////////
void ActorComponent_Sprite::reset(Actor_Base* t_owner) {}

////////////////////////////////////////////////////////////
void ActorComponent_Sprite::draw(Actor_Base* t_owner) {
	if (!m_visible) { return; }
	t_owner->getContext().m_window.draw(m_sprite);
}