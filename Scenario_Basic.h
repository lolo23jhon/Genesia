#ifndef SCENARIO_BASIC_H
#define SCENARIO_BASIC_H

#include "Scenario_Base.h"

class Scenario_Basic : public Scenario_Base {
	   
	void init(SharedContext& t_context);
	void update();
};

#endif // !SCENARIO_BASIC_H