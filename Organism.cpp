#include <exception>
#include "Organism.h"
#include "Food.h"
#include "MathHelpers.h"
#include "Utilities.h"
#include "SharedContext.h"
#include "MathHelpers.h"
#include "Engine.h"
#include "Scenario_Basic.h"
#include "PreprocessorDirectves.h"

static const float S_TIME_ZERO{ 0.f };
static const std::string S_DEFAULT_TEXTURE{ "Texture_organism" };
static const float S_TEXT_OFFSET_FACTOR{ 1.1f };
static const sf::Color S_DEFAULT_COLOR{ 255,255,255,255 };
static const float S_DEFAULT_SIZE{ 1.f };
static const float S_DEFAULT_DESTRUCTION_DELAY{ 10.f };
static const sf::Color S_DEATH_COLOR{ 70,60,50 };

////////////////////////////////////////////////////////////
Organism::Organism(
	SharedContext& t_context,
	const std::string& t_name,
	const sf::Vector2f& t_position,
	const float& t_rotation,
	const float& t_age) :
	Actor_Base(t_context, t_position, t_rotation, S_DEFAULT_COLOR, S_DEFAULT_TEXTURE, sf::IntRect(), true, true),
	m_name{ t_name },
	m_age{ t_age },
	m_ai{ std::make_unique<Ai_Organism>(t_context) },
	m_wasEaten{ false },
	m_destructionDelay{ S_DEFAULT_DESTRUCTION_DELAY },
	m_scenario{ &t_context.m_engine->getScenario() }
{
	m_actorType = ActorType::Organism;
	m_text.setCharacterSize(10U);
	setTextString(m_name);
}

////////////////////////////////////////////////////////////
OrganismPtr Organism::makeDefaultClone(SharedContext& t_context, const std::string& t_name, const sf::Vector2f& t_position, const float& t_rotation, const float& t_age) {
	auto o{ std::make_unique<Organism>(t_context, t_name, t_position, t_rotation, t_age) };
	for (const auto& it : Trait_Base::getVitalTraits()) {
		o->m_traits.addTrait(std::move(Trait_Base::cloneDefaultTrait(it)));
	}
	o->m_traits.onOrganismConstruction(o.get(), 0.f); // Update all the traits
	return std::move(o);
}

////////////////////////////////////////////////////////////
OrganismPtr Organism::makeDefaultOffspring(SharedContext& t_context, const std::string& t_name, const sf::Vector2f& t_position, const float& t_rotation, const float& t_age) {
	auto o{ std::make_unique<Organism>(t_context, t_name, t_position, t_rotation, t_age) };
	for (const auto& it : Trait_Base::getVitalTraits()) {
		o->m_traits.addTrait(std::move(Trait_Base::reproduceDefaultTrait(t_context, it)));
	}
	o->m_traits.onOrganismConstruction(o.get(), 0.f); // Update all the traits
	return std::move(o);
}

////////////////////////////////////////////////////////////
const HSL& Organism::getColorHSL()const { return m_hslColor; }

////////////////////////////////////////////////////////////
void Organism::setColorHSL(const float& t_h, const float& t_s, const float& t_l) {
	m_hslColor.Hue = t_h;
	m_hslColor.Saturation = t_h;
	m_hslColor.Luminance = t_l;
	m_color = m_hslColor.TurnToRGB();
}

////////////////////////////////////////////////////////////
const sf::Color& Organism::getColorRGB()const { return m_color; }

////////////////////////////////////////////////////////////
void Organism::setColorRGB(const sf::Color& t_color) {
	m_color = t_color;
	m_hslColor = HSL::TurnToHSL(t_color);
}

////////////////////////////////////////////////////////////
void Organism::setColor(const sf::Color& t_color) { setColorRGB(t_color); }

////////////////////////////////////////////////////////////
const float& Organism::getMovementSpeed()const { return m_trait_movementSpeed; }

////////////////////////////////////////////////////////////
void Organism::setMovementSpeed(const float& t_movementSpeed) { m_trait_movementSpeed = t_movementSpeed; }

////////////////////////////////////////////////////////////
const float& Organism::getRotationSpeed()const { return m_trait_turningSpeed; }

////////////////////////////////////////////////////////////
void Organism::setRotationSpeed(const float& t_rotationSpeed) { m_trait_turningSpeed = t_rotationSpeed; }

////////////////////////////////////////////////////////////
const float& Organism::getAge()const { return m_age; }

////////////////////////////////////////////////////////////
float Organism::getAgePct() const { return m_age / m_trait_lifespan; }

////////////////////////////////////////////////////////////
void Organism::setAge(const float& t_age) { m_age = t_age; }

////////////////////////////////////////////////////////////
const float& Organism::getSize()const { return m_trait_size; }

////////////////////////////////////////////////////////////
void Organism::setSize(const float& t_size) { m_trait_size = t_size; }

////////////////////////////////////////////////////////////
const std::string& Organism::getName()const { return m_name; }

////////////////////////////////////////////////////////////
void Organism::setName(const std::string& t_name) { m_name = t_name; }

////////////////////////////////////////////////////////////
void Organism::setEnergy(const float& t_energy) { m_energy = t_energy; }

////////////////////////////////////////////////////////////
void Organism::addEnergy(const float& t_energy) { m_energy += t_energy; if (m_energy >= m_trait_maxEnergy) { m_energy = m_trait_maxEnergy; } }

////////////////////////////////////////////////////////////
const float& Organism::getEnergy()const { return m_energy; }

////////////////////////////////////////////////////////////
const float& Organism::getMass()const { return m_mass; }

////////////////////////////////////////////////////////////
const float Organism::getRestingMetabolicRate()const { return m_trait_restingMetabolicRate * m_mass; }

////////////////////////////////////////////////////////////
float Organism::getEnergyPct()const { return m_energy / m_trait_maxEnergy; }

////////////////////////////////////////////////////////////
void Organism::setEnergyPct(const float& t_pct) {
	m_energy = m_trait_maxEnergy * t_pct;
	if (m_energy > m_trait_maxEnergy) { m_energy = m_trait_maxEnergy; }
	else if (m_energy < 0.f) { m_energy = 0.f; }
}

////////////////////////////////////////////////////////////
void Organism::addEnergyPct(const float& t_pct) {
	m_energy += m_trait_maxEnergy * t_pct;
	if (m_energy > m_trait_maxEnergy) { m_energy = m_trait_maxEnergy; }
	else if (m_energy < 0.f) { m_energy = 0.f; }
}

////////////////////////////////////////////////////////////
bool Organism::getWasEaten() const { return m_wasEaten; }

////////////////////////////////////////////////////////////
void Organism::setWasEaten(bool t_wasEaten) { m_wasEaten = t_wasEaten; }

////////////////////////////////////////////////////////////
void Organism::move(const float& t_dx, const float& t_dy) {
	float energyExpediture{ std::sqrtf(t_dx * t_dx + t_dy * t_dy) * m_mass };
	m_energy -= energyExpediture; // Movement costs energy: diplacement * mass
	m_scenario->addEnergy(energyExpediture); // Return heat energy to environment
	Actor_Base::move(t_dx, t_dy);
}

////////////////////////////////////////////////////////////
void Organism::rotate(const float& t_deg) {
	float energyExpediture{ std::fabs(mat::toRadians(t_deg)) * m_mass };
	m_energy -= energyExpediture;
	m_scenario->addEnergy(energyExpediture); // Return heat energy to environment
	Actor_Base::rotate(t_deg);
}

////////////////////////////////////////////////////////////
void Organism::update(const float& t_elapsed) {
	// Update the organism's age
	m_age += t_elapsed;


	// Decrease the organism's energy based on its metabolic rate
	float energyExpediture{ m_trait_restingMetabolicRate * t_elapsed };
	m_energy -= energyExpediture;
	m_scenario->addEnergy(energyExpediture); // Return heat energy to environment

	if ((m_age >= m_trait_lifespan || m_energy <= 0.f) && !m_isDead) { die(); }
	if (m_isDead) {
		if (m_destructionDelay <= 0.f) { m_destroy = true; }
		m_destructionDelay -= t_elapsed;
		return;
	}// Dead organisms do not move, they just wait to decompose.

	// Update all the traits' effects
	m_traits.update(this, t_elapsed);

	// Update the organim's ai
	m_ai->update(this, t_elapsed);

#if defined(_DEBUG) && IS_DISPLAY_ORGNAISMS_DEBUG_TEXT == 1
	setTextString(m_name +
		"\nEnergy: " + std::to_string(static_cast<unsigned>(m_energy >= 0.f ? m_energy : 0.f)) + " / " + std::to_string(static_cast<unsigned>(m_trait_maxEnergy)) +
		"\nAge   : " + std::to_string(static_cast<unsigned>(m_age)) + " / " + std::to_string(static_cast<unsigned>(m_trait_lifespan)) +
		"\nSize  : " + std::to_string(m_trait_size) +
		"\nMass  : " + std::to_string(m_mass) +
		"\nRMR   : " + std::to_string(m_rmr) +
		"\nMovSp : " + std::to_string(m_trait_movementSpeed) +
		"\nRotSp : " + std::to_string(m_trait_turningSpeed) +
		"\nDigEff: " + std::to_string(m_trait_digestiveEfficiency));
#endif // defined(_DEBUG) && IS_DISPLAY_ORGNAISMS_DEBUG_TEXT == 1

	// Lower level update (position, rotation ...)
	Actor_Base::update(t_elapsed);

	// Dynamic color that changes with age
	auto agePct{ getAgePct() };
	if (!m_isDead && agePct >= 0.f && agePct <= 0.7f) {
		auto colorHSL{ getColorHSL() };
		colorHSL.Luminance = (1.f - agePct) * 100.f;
		m_sprite.setColor(colorHSL.TurnToRGB());
	}

	// Update the collider component
	updateCollider();
}

////////////////////////////////////////////////////////////
ActorPtr Organism::clone() {
	auto o{ std::make_unique<Organism>(m_context, m_name, m_position, m_rotation, m_age) };
	o->m_traits = std::move(*m_traits.clone().release()); // Pass unique ptr to regular member
	o->m_traits.onOrganismConstruction(o.get(), 0.f); // Update "OnConstruction" traits
	return std::move(o);
}

////////////////////////////////////////////////////////////
ActorPtr Organism::reproduce(SharedContext& t_context) {
	auto o{ std::make_unique<Organism>(m_context, m_name, m_position, m_rotation, 0.f) }; // Reset the organism's age
	o->m_traits = std::move(*m_traits.reproduce(t_context).release());
	o->m_traits.onOrganismConstruction(o.get(), 0.f);
	return std::move(o);
}

////////////////////////////////////////////////////////////
void Organism::die() {
	m_isDead = true;
	setColorRGB(S_DEATH_COLOR);
	m_sprite.setColor(S_DEATH_COLOR);
	m_name += " (dead)";
	setTextString(m_name);
}

////////////////////////////////////////////////////////////
bool Organism::isAlive() const{	return !m_isDead;}

////////////////////////////////////////////////////////////
void Organism::eat(Food* t_food) {
	if (!t_food || t_food->shouldBeDestroyed()) { return; } // Trying to eat ghost food doesn't work at this level of conciousness.
	if (m_energy > 0.8f * m_trait_maxEnergy) { return; } // Ogranisms at and over 80% of energy are not experiencing hunger

	float foodEnergy{ t_food->getEnergy() };
	float deltaEnergy{ foodEnergy * m_trait_digestiveEfficiency }; // Get the energy boost affected by digestive efficiency

	m_energy += deltaEnergy;
	m_scenario->addEnergy(foodEnergy - deltaEnergy); // Return the energy to the rest of the energy to the environment

	if (m_energy > m_trait_maxEnergy) {
		m_scenario->addEnergy(m_energy - m_trait_maxEnergy); // Return aswell any energy from overeating
		m_energy = m_trait_maxEnergy;
	}
	t_food->setWasEaten(true); // State that the food was eaten, so that it does not try to return its energy itelf, its the organism's task now.
	t_food->setShouldBeDestroyed(true); // Tell the engine the food no longer exists (ha)
}

////////////////////////////////////////////////////////////
void Organism::eat(Organism* t_prey) {
	if (!t_prey || t_prey->shouldBeDestroyed() || t_prey->m_wasEaten) { return; }
	if (m_energy > 0.8f * m_trait_maxEnergy) { return; }

	float foodEnergy{ t_prey->getEnergy() };
	float deltaEnergy{ foodEnergy * m_trait_digestiveEfficiency };

	m_energy += deltaEnergy;
	m_scenario->addEnergy(foodEnergy - deltaEnergy);
	if (m_energy > m_trait_maxEnergy) {
		m_scenario->addEnergy(m_energy - m_trait_maxEnergy); // Return aswell any energy from overeating		
		m_energy = m_trait_maxEnergy;
	}
	t_prey->setWasEaten(true);
	t_prey->setShouldBeDestroyed(true);

}

////////////////////////////////////////////////////////////
void Organism::updateCollider() {
	auto aabb{ m_sprite.getLocalBounds() };
	m_collider->update(this, m_position, { aabb.width * m_trait_size, aabb.height * m_trait_size });
}

////////////////////////////////////////////////////////////
float Organism::getRadius()const { return Actor_Base::getRadius() * m_trait_size; }

////////////////////////////////////////////////////////////
bool Organism::canSpawn(SharedContext& t_context)const {
	return m_scenario->getEnergy() >= m_energy; // Make sure there is enough energy in the environment for it to spawn
}
////////////////////////////////////////////////////////////
void Organism::onSpawn() { m_scenario->addEnergy(-m_energy); } // Capture energy from the environment

////////////////////////////////////////////////////////////
void Organism::onDestruction(SharedContext& t_context) {
	if (!m_wasEaten) {
		m_scenario->addEnergy(m_energy);
	} // Return the energy to the environment
	Actor_Base::onDestruction(t_context);
}