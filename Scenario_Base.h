#ifndef SCENARIO_BASE_H
#define SCENARIO_BASE_H

struct SharedContext;

class Scenario_Base {

	unsigned m_maxNumActors; // Unlimited if 0

public:
	Scenario_Base();
	unsigned getMaxNumActors()const;
	void setMaxNumActors(const unsigned& t_num);

	virtual void init(SharedContext & t_context) = 0; // Anything that happens when the simulation begins, like spawning the first characters
	virtual void update() = 0; // Anything that happens every frame, such as spawning x actor on condition y at time z

};

#endif // !SCENARIO_BASE_H