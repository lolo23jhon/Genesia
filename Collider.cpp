#include "Collider.h"
#include <SFML/Graphics/RectangleShape.hpp>

static const sf::Color S_RECT_SHAPE_FILL_COLOR{ 0,255,0,30 };
static const sf::Color S_RECT_SHAPE_OUTLINE_COLOR{ 0,255,100,200 };
static const float S_RECT_SHAPE_OUTLINE_THICKNESS{ 1.f };
static const sf::RectangleShape S_RECT_SHAPE{ []() {
	sf::RectangleShape rect({0.f,0.f});
	rect.setFillColor({S_RECT_SHAPE_FILL_COLOR});
	rect.setOutlineColor(S_RECT_SHAPE_OUTLINE_COLOR);
	rect.setOutlineThickness(S_RECT_SHAPE_OUTLINE_THICKNESS);
	return std::move(rect);
}() };

////////////////////////////////////////////////////////////
Collider::Collider(Actor_Base* t_owner, const sf::Vector2f& t_position, const sf::Vector2f& t_size, bool t_isPosCenter) : m_aabb{ sf::FloatRect() }, m_colliderType{ ColliderType::AABB }, m_owner{ t_owner }
{
	if (t_isPosCenter) { setCenterPos(t_position); }
	else { setTopLeftPos(t_position); }
	setSize(t_size);
}

////////////////////////////////////////////////////////////
void Collider::update(Actor_Base* t_owner, const sf::Vector2f& t_pos, const sf::Vector2f& t_size, bool t_isPosCenter) {
	m_owner = t_owner;
	setSize(t_size);
	if (t_isPosCenter) { setCenterPos(t_pos); }
	else { setTopLeftPos(t_pos); }
}

////////////////////////////////////////////////////////////
void Collider::draw(sf::RenderWindow& t_window) {
	sf::RectangleShape rect{ S_RECT_SHAPE };
	rect.setSize({ m_aabb.width, m_aabb.height });
	rect.setPosition(getTopLeftPos());
	t_window.draw(rect);
}

////////////////////////////////////////////////////////////
void Collider::setSize(const float& t_width, const float& t_height) {
	m_aabb.width = t_width;
	m_aabb.height = t_height;
}

////////////////////////////////////////////////////////////
void Collider::setSize(const sf::Vector2f& t_size) {
	m_aabb.width = t_size.x;
	m_aabb.height = t_size.y;
}

////////////////////////////////////////////////////////////
void Collider::setTopLeftPos(const float& t_x, const float& t_y) {
	m_aabb.left = t_x;
	m_aabb.top = t_y;
}

////////////////////////////////////////////////////////////
void Collider::setTopLeftPos(const sf::Vector2f& t_pos) {
	m_aabb.left = t_pos.x;
	m_aabb.top = t_pos.y;
}

////////////////////////////////////////////////////////////
void Collider::setCenterPos(const float& t_x, const float& t_y) {
	m_aabb.left = t_x - m_aabb.width * 0.5f;
	m_aabb.top = t_y - m_aabb.height * 0.5f;
}

////////////////////////////////////////////////////////////
void Collider::setCenterPos(const sf::Vector2f& t_pos) {
	m_aabb.left = t_pos.x - m_aabb.width * 0.5f;
	m_aabb.top = t_pos.y - m_aabb.height * 0.5f;
}

////////////////////////////////////////////////////////////
sf::Vector2f Collider::getTopLeftPos()const {
	return { m_aabb.left, m_aabb.top };
}

////////////////////////////////////////////////////////////
sf::Vector2f Collider::getCenterPos()const {
	return { m_aabb.left + m_aabb.width * 0.5f, m_aabb.top + m_aabb.height * 0.5f };
}

////////////////////////////////////////////////////////////
const sf::FloatRect Collider::getAABB()const { return m_aabb; }

////////////////////////////////////////////////////////////
const Actor_Base* Collider::getOwner()const { return m_owner; }

////////////////////////////////////////////////////////////
Actor_Base* Collider::getOwner() { return m_owner; }

////////////////////////////////////////////////////////////
bool Collider::checkCollision(const Collider* t_otherAABB)const {
	return m_aabb.intersects(t_otherAABB->m_aabb);
}

////////////////////////////////////////////////////////////
const ColliderType& Collider::getColliderType()const { return m_colliderType; }
