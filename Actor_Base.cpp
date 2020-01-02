#include "Actor_Base.h"
#include "ActorComponent_Base.h"
#include "ActorComponent_Drawable.h"
#include "MathHelpers.h"


////////////////////////////////////////////////////////////
Actor_Base::Actor_Base(const SharedContext& t_context,const sf::Vector2f& t_position, const float& t_rotationDeg) :
	m_context{t_context}, m_position{ t_position }, m_rotation{ t_rotationDeg }
{
	init();
}

////////////////////////////////////////////////////////////
SharedContext& Actor_Base::getContext() { return m_context; }


////////////////////////////////////////////////////////////
void Actor_Base::init() {

	// <-- TODO: Solve component initiliazation

	// Put all drawable components in the drawing deque
	if (!m_drawables.empty()) { m_drawables.clear(); }
	for (auto& comp : m_components) {
		if (ActorComponent_Base::isComponentDrawable(comp.first)) {
			m_drawables.push_back(dynamic_cast<ActorComponent_Drawable*>(comp.second.get()));
		}
	}
}

////////////////////////////////////////////////////////////
void Actor_Base::update() {
	for (auto& it : m_components) {
		it.second->update(this);
	}
}

////////////////////////////////////////////////////////////
void Actor_Base::reset() {
	for (auto& it : m_components) {
		it.second->reset(this);
	}
}

////////////////////////////////////////////////////////////
void Actor_Base::draw() {
	for (auto& it : m_drawables) {
		if (it->getVisibility()) { it->draw(this); }
	}
}

////////////////////////////////////////////////////////////
const sf::Vector2f& Actor_Base::getPosition()const { return m_position; }

////////////////////////////////////////////////////////////
const float& Actor_Base::getRotation()const { return m_rotation; }

////////////////////////////////////////////////////////////
void Actor_Base::setPosition(const float& t_x, const float& t_y) { m_position.x = t_x; m_position.y = t_y; }

////////////////////////////////////////////////////////////
void Actor_Base::setRotation(const float& t_r) {
	m_rotation = mat::normalizeAngle(t_r);
}

////////////////////////////////////////////////////////////
void Actor_Base::rotate(const float& t_dr) {
	m_rotation = mat::normalizeAngle(m_rotation + t_dr);
}

////////////////////////////////////////////////////////////
void Actor_Base::incrementPosition(const float& t_dx, const float& t_dy) { m_position.x += t_dx; m_position.y += t_dy; }


////////////////////////////////////////////////////////////
bool Actor_Base::insertComponent(const ActorComponentType& t_componentType, std::unique_ptr<ActorComponent_Base> t_component) {
	auto it{ m_components.emplace(t_componentType,std::move(t_component)) };
	return it.second;
}

////////////////////////////////////////////////////////////
void Actor_Base::forceInsertComponent(const ActorComponentType& t_componentType, std::unique_ptr<ActorComponent_Base> t_component) {
	auto it{ m_components.find(t_componentType) };
	if (it == m_components.end()) {
		it->second = std::move(t_component);
	}
	else {
		m_components.emplace(t_componentType, std::move(t_component));
	}
}

////////////////////////////////////////////////////////////
bool Actor_Base::removeComponent(const ActorComponentType& t_componentType) {
	auto it{ m_components.find(t_componentType) };
	if (it == m_components.end()) { return false; }
	m_components.erase(it);
	return true;
}

////////////////////////////////////////////////////////////
bool Actor_Base::hasComponent(const ActorComponentType& t_componentType)const {
	return (m_components.find(t_componentType) == m_components.cend());
}

////////////////////////////////////////////////////////////
void Actor_Base::purgeComponents() {
	m_components.clear();
}

////////////////////////////////////////////////////////////
std::unique_ptr<ActorComponent_Base> Actor_Base::extractComponent(const ActorComponentType& t_componentType) {
	auto it{ m_components.find(t_componentType) };
	if (it == m_components.end()) { return nullptr; }
	auto component{ std::move(it->second) };
	m_components.erase(it);
	return std::move(component);
}

////////////////////////////////////////////////////////////
void Actor_Base::swapComponent(const ActorComponentType& t_componentType, Actor_Base* t_act1, Actor_Base* t_act2) {
	if (t_act1 == t_act2) { return; }
	auto act1_comp{ t_act1->extractComponent(t_componentType) };
	auto act2_comp{ t_act2->extractComponent(t_componentType) };
	if (act1_comp) {
		t_act2->insertComponent(t_componentType, std::move(act1_comp));
	}
	if (act2_comp) {
		t_act1->insertComponent(t_componentType, std::move(act2_comp));
	}
}

////////////////////////////////////////////////////////////
ActorComponent_Base* Actor_Base::seeComponent(const ActorComponentType& t_componentType) {
	auto it{ m_components.find(t_componentType) };
	if (it == m_components.end()) { return nullptr; }
	return it->second.get();
}

////////////////////////////////////////////////////////////
const ActorComponent_Base* Actor_Base::seeComponent(const ActorComponentType& t_componentType)const {
	auto it{ m_components.find(t_componentType) };
	if (it == m_components.cend()) { return nullptr; }
	return it->second.get();
}

////////////////////////////////////////////////////////////
void Actor_Base::sendDrawableToFront(const ActorComponent_Drawable* t_component) {
	if (!t_component) { return; }
	auto it{ std::find_if(m_drawables.begin(), m_drawables.end(),
		[&t_component](const ActorComponent_Drawable* t_ptr) {return t_ptr == t_component; }) };
	auto tmp_ptr{ *it };
	m_drawables.erase(it);
	m_drawables.push_front(tmp_ptr);
}

void Actor_Base::sendDrawableToBack(const ActorComponent_Drawable* t_component) {
	if (!t_component) { return; }
	auto it{ std::find_if(m_drawables.begin(), m_drawables.end(),
		[&t_component](const ActorComponent_Drawable* t_ptr) {return t_ptr == t_component; }) };
	auto tmp_ptr{ *it };
	m_drawables.erase(it);
	m_drawables.push_back(tmp_ptr);
}