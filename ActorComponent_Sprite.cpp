#include <cassert>
#include "ActorComponent_Sprite.h"
#include "Actor_Base.h"
#include "ResourceHolder.h"
#include "MathHelpers.h"
#include "SharedContext.h"


////////////////////////////////////////////////////////////
ActorComponent_Sprite::ActorComponent_Sprite(ResourceHolder& t_resHolder, const std::string& t_texture, const sf::IntRect& t_spriteRect): 
	ActorComponent_Drawable(), m_texture{ t_texture }, m_textureRect{t_spriteRect}
{
	Resource* resource{ t_resHolder.getResource(ResourceType::Texture, t_texture) };
	assert(resource != nullptr && "ActorComponenet_Sprite::ActorComponenet_Sprite -> ResourceHolder returned nullptr from invalid texture name!");
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


////////////////////////////////////////////////////////////
bool ActorComponent_Sprite::setSprite(Actor_Base* t_owner, const std::string& t_texture, const sf::IntRect& t_spriteRect) {
	Resource* resource{ t_owner->getContext().m_resourceHolder.getResource(ResourceType::Texture, t_texture) };
	if (!resource) { return false; }
	m_sprite.setTexture(std::get<sf::Texture>(*resource));
	m_sprite.setTextureRect(t_spriteRect);
	return true;
}

////////////////////////////////////////////////////////////
void ActorComponent_Sprite::changeTextureRect(sf::IntRect t_spriteRect) {
	m_sprite.setTextureRect(t_spriteRect);
}

////////////////////////////////////////////////////////////
ActorComponentPtr ActorComponent_Sprite::clone(SharedContext& t_context)const {
	return std::make_unique<ActorComponent_Sprite>(t_context.m_resourceHolder, m_texture, m_textureRect);
}

////////////////////////////////////////////////////////////
const sf::IntRect& ActorComponent_Sprite::getTextureRect()const { return m_textureRect; }

////////////////////////////////////////////////////////////
void ActorComponent_Sprite::setTextureRext(const sf::IntRect& t_textureRect) { m_textureRect = t_textureRect; }