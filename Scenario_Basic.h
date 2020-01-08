#ifndef SCENARIO_BASIC_H
#define SCENARIO_BASIC_H

#include "Scenario_Base.h"
#include "Actor_Base.h"

class Organism;

class Scenario_Basic : public Scenario_Base {

	std::unique_ptr<Organism> m_firstOrganism;

public:
	Scenario_Basic(SharedContext& t_context, const unsigned& t_maxNumActors, const unsigned& t_initialNumOrganisms, const float& t_width = 1000.f, const float& t_height= 1000.f);
	void init();


};

#endif // !SCENARIO_BASIC_H