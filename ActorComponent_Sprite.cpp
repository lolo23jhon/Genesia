#include "ActorComponent_Sprite.h"
#include "Actor_Base.h"
#include "ResourceHolder.h"
#include "MathHelpers.h"


////////////////////////////////////////////////////////////
ActorComponent_Sprite::ActorComponent_Sprite(Actor_Base* t_owner, const std::string& t_texture, const sf::IntRect& t_spriteRect) :
	ActorComponent_Drawable(), m_texture{t_texture}
{	
	Resource* resource{ t_owner->getContext().m_resourceHolder.getResource(ResourceType::Texture, t_texture) };
	
	m_sprite.setTexture(std::get<sf::Texture>(*resource));
	m_sprite.setTextureRect(t_spriteRect);
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