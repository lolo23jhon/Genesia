#include "Food.h"
#include <exception>
#include "SharedContext.h"
#include "ResourceHolder.h"
#include "CollisionManager.h"
#include "Engine.h"
#include "Scenario_Basic.h"

static const std::string S_FOOD_TEXTURE{ "Texture_food" };
static const sf::Color S_FOOD_COLOR{ 200,255,22,255 };
static const float S_INFINITY{ INFINITY };
static const float S_SPAWN_ANIM_DURATION{ 1.f };

const float Food::S_BASE_ENERGY{ 500.f };

////////////////////////////////////////////////////////////
unsigned Food::s_numFood{ 0U };

////////////////////////////////////////////////////////////
unsigned Food::getNumFood() { return s_numFood; }

////////////////////////////////////////////////////////////
bool Food::canSpawn(SharedContext& t_context) const { return t_context.m_engine->getScenario().getEnergy() >= m_energy; } // Check if there is enough energy in the environment to spawn

////////////////////////////////////////////////////////////
void Food::onSpawn(SharedContext& t_context) {
	s_numFood++;

	// Subtract energy from the environment to spawn
	t_context.m_engine->getScenario().addEnergy(-m_energy);
}

////////////////////////////////////////////////////////////
void Food::onDestruction(SharedContext& t_context) {
	s_numFood--;

	// If the food was eaten, the organism is responsible of returning the energy, else the food is.
	if (!m_wasEaten) { t_context.m_engine->getScenario().addEnergy(m_energy); }
}

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
	m_age{ 0.f },
	m_wasEaten{ false }
{
	m_actorType = ActorType::Food;
	m_sprite.setColor(S_FOOD_COLOR);
	m_color = S_FOOD_COLOR;
	setTextString("Food: " + std::to_string(static_cast<int>(m_energy)));
	m_text.setCharacterSize(10U);
	updateCollider();
}

////////////////////////////////////////////////////////////
const float& Food::getEnergy()const { return m_energy; }

////////////////////////////////////////////////////////////
void Food::setEnergy(const float& t_energy) {
	m_energy = t_energy;
	m_sprite.setScale(m_energy / S_BASE_ENERGY, m_energy / S_BASE_ENERGY);
}

////////////////////////////////////////////////////////////
const float& Food::getAge()const { return m_age; }

////////////////////////////////////////////////////////////
void Food::setAge(const float& t_age) { m_age = t_age; }

////////////////////////////////////////////////////////////
const float& Food::getDuration()const { return m_hasUnlimitedDuration ? S_INFINITY : m_duration; }

////////////////////////////////////////////////////////////
void Food::setDuration(const float& t_duration) { m_duration = t_duration; if (!m_duration) { m_hasUnlimitedDuration = false; } }

////////////////////////////////////////////////////////////
void Food::setWasEaten(bool t_wasEaten) { m_wasEaten = t_wasEaten; }

////////////////////////////////////////////////////////////
void Food::update(const float& t_elapsed) {
	m_age += t_elapsed;

	// Fade in spawn animation
	if (m_age < S_SPAWN_ANIM_DURATION) {
		m_sprite.setScale((m_energy / S_BASE_ENERGY) * m_age / S_SPAWN_ANIM_DURATION, (m_energy / S_BASE_ENERGY) * m_age / S_SPAWN_ANIM_DURATION);
		m_color.a = static_cast<sf::Uint8>(255 * m_age / S_SPAWN_ANIM_DURATION);
	}

	if (m_age >= m_duration && !m_hasUnlimitedDuration) {
		m_destroy = true; s_numFood--;
	}
	Actor_Base::update(t_elapsed);
	updateCollider();
}

////////////////////////////////////////////////////////////
void Food::updateCollider() {
	auto aabb{ m_sprite.getLocalBounds() };
	m_collider->update(this, m_position, { aabb.width, aabb.height });
}


////////////////////////////////////////////////////////////
ActorPtr Food::clone(SharedContext& t_context) {
	return std::make_unique<Food>(t_context, m_position, m_rotation, m_energy, m_duration, m_isSpriteVisible, m_isTextVisible);
}