#ifndef FOOD_H
#define FOOD_H

#include "Actor_Base.h"

class Food : public Actor_Base {

	float m_energy;
	float m_nutrition;
	float m_age;

public:
	Food(SharedContext& t_context,
		const sf::Vector2f& t_position,
		const float& t_rotation,
		const sf::Color& t_color,
		const std::string& t_texture,
		const sf::IntRect& t_spriteRect,
		const float& t_energy,
		const float& t_nutrition,
		bool t_isSpriteVisible = true,
		bool t_isTextVisible = true);

	const float& getEnergy()const;
	void setEnergy(const float& t_energy);
	const float& getNutrition()const;
	void setNutrition(const float& t_nutrition);
	const float& getAge()const;
	void setAge(const float& t_age);

};

#endif // !FOOD_H