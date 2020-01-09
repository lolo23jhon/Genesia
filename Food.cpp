#include "Food.h"

////////////////////////////////////////////////////////////
Food::Food(SharedContext& t_context, const sf::Vector2f& t_position,
	const float& t_rotation, const sf::Color& t_color,
	const std::string& t_texture, const sf::IntRect& t_spriteRect,
	const float& t_energy, const float& t_nutrition,
	bool t_isSpriteVisible, bool t_isTextVisible) :
	Actor_Base(t_context, t_position, t_rotation, t_color, t_texture, t_spriteRect, t_isSpriteVisible, t_isTextVisible),
	m_energy{ t_energy },
	m_nutrition{ t_nutrition },
	m_age{ 0.f }
{
}

////////////////////////////////////////////////////////////
const float& Food::getEnergy()const { return m_energy; }

////////////////////////////////////////////////////////////
void Food::setEnergy(const float& t_energy) { m_energy = t_energy; }

////////////////////////////////////////////////////////////
const float& Food::getNutrition()const { return m_nutrition; }

////////////////////////////////////////////////////////////
void Food::setNutrition(const float& t_nutrition) { m_nutrition = t_nutrition; }

////////////////////////////////////////////////////////////
const float& Food::getAge()const { return m_age; }

////////////////////////////////////////////////////////////
void Food::setAge(const float& t_age) { m_age = t_age; }