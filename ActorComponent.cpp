#include "ActorComponent.h"
#include "Actor_Base.h"
#include "MathHelpers.h"

////////////////////////////////////////////////////////////
bool ActorComponent_Base::isComponentDrawable(const ActorComponentType& t_componentId) {
	switch (t_componentId) {
	case ActorComponentType::Ai:
		return false;
	case ActorComponentType::Box:
	case ActorComponentType::Sprite:
	case ActorComponentType::Text:
	default:
		return true;
	}
}

////////////////////////////////////////////////////////////
ActorComponent_Drawable::ActorComponent_Drawable() {};

////////////////////////////////////////////////////////////
void ActorComponent_Drawable::setVisibility(const bool& t_isVisible) { m_visible = t_isVisible; }

////////////////////////////////////////////////////////////
bool ActorComponent_Drawable::getVisibility()const { return m_visible; }

////////////////////////////////////////////////////////////
void ActorComponent_Drawable::setPositionOffsetFromOwner(const float& t_x, const float& t_y) {
	m_positionOffsetFromOwner.x = t_x;
	m_positionOffsetFromOwner.y = t_y;
}

////////////////////////////////////////////////////////////
void ActorComponent_Drawable::setPositionOffsetFromOwner(const sf::Vector2f& t_offset) {
	m_positionOffsetFromOwner = t_offset;
}

////////////////////////////////////////////////////////////
void ActorComponent_Drawable::setPolarPositionOffsetFromOwner(const float& t_distance, const float& t_angleDeg) {
	mat::to_cartesian(t_distance, t_angleDeg, m_positionOffsetFromOwner.x, m_positionOffsetFromOwner.y);
}


////////////////////////////////////////////////////////////
void ActorComponent_Drawable::incrementPolarDistanceFromOwner(const float& t_dist) {

}

////////////////////////////////////////////////////////////
void ActorComponent_Drawable::incrementPositionOffsetFromOwner(const float& t_x, const float& t_y) {
	m_positionOffsetFromOwner.x += t_x;
	m_positionOffsetFromOwner.y += t_y;
}

////////////////////////////////////////////////////////////
void ActorComponent_Drawable::incrementPositionOffsetFromOwner(const sf::Vector2f& t_deltaOffset) {
	m_positionOffsetFromOwner += t_deltaOffset;
}

////////////////////////////////////////////////////////////
void ActorComponent_Drawable::setAngularOffsetFromOwner(const float& t_angDeg) {
	m_angularOffsetFromOwner = mat::normalizeAngle(m_angularOffsetFromOwner + t_angDeg);

}

////////////////////////////////////////////////////////////
const float& ActorComponent_Drawable::getAngularOffsetFromOwner()const {
	return m_angularOffsetFromOwner;
}

////////////////////////////////////////////////////////////
sf::Vector2f ActorComponent_Drawable::getAboslutePosition(Actor_Base* t_owner)const {
	const auto& ownerPos{ t_owner->getPosition() };
	return { ownerPos.x + m_positionOffsetFromOwner.x, ownerPos.y + m_positionOffsetFromOwner.y };
}

////////////////////////////////////////////////////////////
float ActorComponent_Drawable::getAbsoluteRotation(Actor_Base* t_owner)const {
	return mat::normalizeAngle(t_owner->getRotation() + m_angularOffsetFromOwner);
}