#include "Collider_Circle.h"
#include <SFML/Graphics/CircleShape.hpp>
#include "MathHelpers.h"

static const sf::Color S_CIRCLE_SHAPE_FILL_COLOR{ 0,0,255,30 };
static const sf::Color S_CIRCLE_OUTLINE_COLOR{ 0,100,255,100 };
static const float S_CIRCLE_SHAPE_OULTINE_THICKNESS{ 1.f };

static const sf::CircleShape S_CIRCLE_SHAPE{ []() {
	sf::CircleShape circle;
	circle.setFillColor(S_CIRCLE_SHAPE_FILL_COLOR);
	circle.setOutlineColor(S_CIRCLE_OUTLINE_COLOR);
	circle.setOutlineThickness(S_CIRCLE_SHAPE_OULTINE_THICKNESS);
	return std::move(circle);
}() };

////////////////////////////////////////////////////////////
Collider_Circle::Collider_Circle(Actor_Base* t_owner, const sf::Vector2f& t_pos, const float& t_radius) :
	Collider(t_owner, t_pos, { t_radius, t_radius }), m_radius{ t_radius }{m_colliderType = ColliderType::CIRCLE; }

////////////////////////////////////////////////////////////
void Collider_Circle::setRadius(const float& t_radius) { m_radius = t_radius; }

////////////////////////////////////////////////////////////
const float& Collider_Circle::getRadius()const { return m_radius; }

////////////////////////////////////////////////////////////
void Collider_Circle::update(Actor_Base* t_owner, const sf::Vector2f& t_pos, const float& t_radius) {
	Collider::update(t_owner, t_pos, { t_radius, t_radius });
}

////////////////////////////////////////////////////////////
void Collider_Circle::draw(sf::RenderWindow& t_window) {
	Collider::draw(t_window);
	sf::CircleShape circle{ S_CIRCLE_SHAPE };
	circle.setPosition(getCenterPos());
	circle.setOrigin({ m_aabb.width * 0.5f, m_aabb.height * 0.5f });
	t_window.draw(circle);
}

////////////////////////////////////////////////////////////
bool  Collider_Circle::checkCollision(const Collider* t_otherCircle)const {
	// Distance between centers is lesser than sum of radii; radius = len /2 (to avoid dynamic cast)
	return (mat::distance(getCenterPos(), t_otherCircle->getCenterPos()) < ((m_aabb.width + t_otherCircle->getAABB().width)* 0.5 ));
}
