#ifndef SCENARIO_BASIC_H
#define SCENARIO_BASIC_H

#include "Scenario_Base.h"
#include "Organism.h"
#include "Food.h"


class Scenario_Basic : public Scenario_Base {

	std::unique_ptr<Organism> m_firstOrganism;
	std::unique_ptr<Food> m_food;
	unsigned m_initialNumOrganisms;
	unsigned m_maxNumOrganisms;
	unsigned m_initialNumFood;
	unsigned m_maxNumFood;

	float m_energyPool; // When something spawns, it draws from the global energy pool of the environment, which is finite and constant
	// Every time energy is spent by an organism, it returns to the environment: digestion, movement, and death. Reproduction instead puts it into the offspring
	float m_maxEnergy; // Just to keep track of the original value instead someone decides to create energy out of nothing (which might aswell be possible in nonduality)
						
						

public:
	Scenario_Basic(SharedContext& t_context, 
		const float& t_energyPool,
		const unsigned& t_initialNumOrganisms,
		const unsigned& t_maxNumOrganisms,
		const unsigned& t_initialNumFood,
		const unsigned& t_maxNumFood, 
		const float& t_width, 
		const float& t_height);
	void init();
	void update(const float& t_elapsed);

	void addEnergy(const float& t_e); // Returns energy from the environment
	const float& getEnergy()const;
};

#endif // !SCENARIO_BASIC_H