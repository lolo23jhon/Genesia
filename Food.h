#ifndef FOOD_H
#define FOOD_H

#include "Actor_Base.h"
#include "Collider.h"

class Food : public Actor_Base {

	static unsigned s_numFood;

	float m_energy; // Energy granted to the eater
	float m_age;
	float m_duration;

	bool m_hasUnlimitedDuration;
	bool m_wasEaten; // Used to discern the energy responsibility. If it was eaten nothing happens, else it returns the energy on destruction.

public:
	Food(SharedContext& t_context,
		const sf::Vector2f& t_position,
		const float& t_rotation,
		const float& t_energy,
		const float& t_duration,
		bool t_isSpriteVisible = true,
		bool t_isTextVisible = true);

	const float& getEnergy()const;
	void setEnergy(const float& t_energy);
	const float& getAge()const;
	void setAge(const float& t_age);
	const float& getDuration()const;
	void setDuration(const float& t_duration);
	void setWasEaten(bool t_wasEaten);

	static unsigned getNumFood();

	bool canSpawn(SharedContext& t_context)const;
	void onSpawn(SharedContext& t_context);
	void onDestruction(SharedContext& t_context);

	void update(const float& t_elapsed);
	void updateCollider();

	ActorPtr clone(SharedContext& t_context);
};

#endif // !FOOD_H