#include <exception>
#include "Organism.h"
#include "MathHelpers.h"
#include "Utilities.h"
#include "SharedContext.h"
#include "Engine.h"

static const std::string S_DEFAULT_TEXTURE{ "Texture_organism" };
static const float S_TEXT_OFFSET_FACTOR{ 1.1f };

////////////////////////////////////////////////////////////
Organism::Organism(
	SharedContext& t_context,
	const std::string& t_name,
	const float& t_size,
	const sf::Color& t_color,
	const sf::Vector2f& t_position,
	const float& t_rotation,
	const float& t_movSpeed,
	const float& t_rotSpeed,
	const float& t_age) :
	Actor_Base(t_context, t_position, t_rotation, t_color, S_DEFAULT_TEXTURE, sf::IntRect(), true, true),
	m_name{ t_name },
	m_size{ t_size },
	m_movementSpeed{ t_movSpeed },
	m_rotationSpeed{ t_rotSpeed },
	m_age{ t_age },
	m_ai{ nullptr }
{
	m_ai = std::make_unique<Ai_Organism>(this);
	const auto& textPos{ m_text.getPosition() };
	// Display text on top of the sprite
	m_text.setPosition(textPos.x, textPos.y - (m_size / 2) * S_TEXT_OFFSET_FACTOR);
	m_text.setString(m_name);
}

////////////////////////////////////////////////////////////
const HSL& Organism::getColorHLS()const { return m_hslColor; }

////////////////////////////////////////////////////////////
void Organism::setColorHSL(const float& t_h, const float& t_s, const float& t_l) {
	m_hslColor.Hue = t_h;
	m_hslColor.Saturation = t_h;
	m_hslColor.Luminance = t_l;
	m_color = m_hslColor.TurnToRGB();
}


////////////////////////////////////////////////////////////
const float& Organism::getMovementSpeed()const { return m_movementSpeed; }

////////////////////////////////////////////////////////////
void Organism::setMovementSpeed(const float& t_movementSpeed) { m_movementSpeed = t_movementSpeed; }

////////////////////////////////////////////////////////////
const float& Organism::getRotationSpeed()const { return m_rotationSpeed; }

////////////////////////////////////////////////////////////
void Organism::setRotationSpeed(const float& t_rotationSpeed) { m_rotationSpeed = t_rotationSpeed; }

////////////////////////////////////////////////////////////
const float& Organism::getAge()const { return m_age; }

////////////////////////////////////////////////////////////
void Organism::setAge(const float& t_age) { m_age = t_age; }

////////////////////////////////////////////////////////////
const float& Organism::getSize()const { return m_size; }

////////////////////////////////////////////////////////////
void Organism::setSize(const float& t_size) { m_size = t_size; }

////////////////////////////////////////////////////////////
const std::string& Organism::getName()const { return m_name; }

////////////////////////////////////////////////////////////
void Organism::setName(const std::string& t_name) { m_name = t_name; }

////////////////////////////////////////////////////////////
void Organism::update(const float& t_elapsed) {
	// Update the organism's age
	m_age += t_elapsed;

	// Update all the traits' effects
	m_traits.update(this, t_elapsed);

	// Update the organim's ai
	m_ai->update(this, t_elapsed);
	
	// Lower level update (position, rotation ...)
	Actor_Base::update(t_elapsed);
}

////////////////////////////////////////////////////////////
ActorPtr Organism::clone() {
	return std::make_unique<Organism>(m_context, m_name, m_size, m_color, m_position, m_rotation, m_movementSpeed, m_rotationSpeed, m_age);
}

////////////////////////////////////////////////////////////
void Organism::die() {
	m_isDead = true;
	m_name += " (dead)";
}