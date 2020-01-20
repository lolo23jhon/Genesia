#include "Food.h"
#include <exception>
#include "SharedContext.h"
#include "ResourceHolder.h"
#include "CollisionManager.h"

static const std::string S_FOOD_TEXTURE{ "Texture_food" };
static const sf::Color S_FOOD_COLOR{ 255,255,255,255 };
static const float S_INFINITY{ INFINITY };

////////////////////////////////////////////////////////////
unsigned Food::s_numFood{ 0U };

////////////////////////////////////////////////////////////
unsigned Food::getNumFood() { return s_numFood; }

////////////////////////////////////////////////////////////
Food::Food(SharedContext& t_context,
	const sf::Vector2f& t_position,
	const float& t_rotation,
	const float& t_energy,
	const float& t_duration,
	bool t_isSpriteVisible,
	bool t_isTextVisible) :
	Actor_Base(t_context, t_position, t_rotation, S_FOOD_COLOR, S_FOOD_TEXTURE, sf::IntRect(), t_isSpriteVisible, t_isTextVisible),
	m_energy{ t_energy },
	m_duration{ t_duration },
	m_hasUnlimitedDuration{ !static_cast<bool>(t_duration) },
	m_age{ 0.f }
{
	s_numFood++;
	m_actorType = ActorType::Food;
	setTextString("Food: " + std::to_string(static_cast<int>(m_energy)));
	updateCollider();
}

////////////////////////////////////////////////////////////
const float& Food::getEnergy()const { return m_energy; }

////////////////////////////////////////////////////////////
void Food::setEnergy(const float& t_energy) { m_energy = t_energy; }

////////////////////////////////////////////////////////////
const float& Food::getAge()const { return m_age; }

////////////////////////////////////////////////////////////
void Food::setAge(const float& t_age) { m_age = t_age; }

////////////////////////////////////////////////////////////
const float& Food::getDuration()const { return m_hasUnlimitedDuration ? S_INFINITY : m_duration; }

////////////////////////////////////////////////////////////
void Food::setDuration(const float& t_duration) { m_duration = t_duration; if (!m_duration) { m_hasUnlimitedDuration = false; } }

////////////////////////////////////////////////////////////
void Food::update(const float& t_elapsed) {
	m_age += t_elapsed;
	if (m_age >= m_duration && !m_hasUnlimitedDuration) {
		m_destroy = true; s_numFood--;
	}
	Actor_Base::update(t_elapsed);
	updateCollider();
}

////////////////////////////////////////////////////////////
void Food::updateCollider() {
	auto aabb{ m_sprite.getLocalBounds() };
	m_collider->update(this, m_position, {aabb.width, aabb.height});
}


////////////////////////////////////////////////////////////
ActorPtr Food::clone(SharedContext& t_context) {
	return std::make_unique<Food>(t_context, m_position, m_rotation, m_energy, m_duration, m_isSpriteVisible, m_isTextVisible);
}