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

public:
	Scenario_Basic(SharedContext& t_context, 
		const unsigned& t_initialNumOrganisms,
		const unsigned& t_maxNumOrganisms,
		const unsigned& t_initialNumFood,
		const unsigned& t_maxNumFood, 
		const float& t_width, 
		const float& t_height);
	void init();
	void update(const float& t_elapsed);


};

#endif // !SCENARIO_BASIC_H