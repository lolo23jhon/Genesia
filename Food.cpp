#include "Food.h"
#include <exception>
#include "SharedContext.h"
#include "ResourceHolder.h"
#include "CollisionManager.h"

static const std::string S_FOOD_TEXTURE{ "Texture_food" };
static const sf::Color S_FOOD_COLOR{ 255,255,255,255 };

////////////////////////////////////////////////////////////
unsigned Food::s_numFood{ 0U };

////////////////////////////////////////////////////////////
const unsigned& Food::getNumFood() { return s_numFood; }

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
	m_age{ 0.f }
{
	setTextString("Food: " + std::to_string(static_cast<int>(m_energy)));
	s_numFood++;
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
const float& Food::getDuration()const { return m_duration; }

////////////////////////////////////////////////////////////
void Food::setDuration(const float& t_duration) { m_duration = t_duration; }

////////////////////////////////////////////////////////////
void Food::update(const float& t_elapsed) {
	m_age += t_elapsed;
	if (m_age >= m_duration) {
		m_destroy = true; s_numFood--;
	}
	Actor_Base::update(t_elapsed);
}

////////////////////////////////////////////////////////////
void Food::updateCollider() {
	auto spriteBounds{ m_sprite.getGlobalBounds() };
	m_collider.update(this, m_position.x, m_position.y, spriteBounds.width, spriteBounds.height, spriteBounds.width * 0.5f);
}


////////////////////////////////////////////////////////////
ActorPtr Food::clone(SharedContext& t_context) {
	return std::make_unique<Food>(t_context, m_position, m_rotation, m_energy, m_duration, m_isSpriteVisible, m_isTextVisible);
}