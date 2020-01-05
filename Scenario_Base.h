#ifndef SCENARIO_BASE_H
#define SCENARIO_BASE_H

struct SharedContext;

class Scenario_Base {
protected:
	unsigned m_maxNumActors; // Unlimited if 0
	unsigned m_initialNumOrganisms;
	SharedContext& m_context;

public:
	Scenario_Base(SharedContext& t_context, const unsigned& t_maxNumActors, const unsigned& t_initialNumOrganisms);
	const unsigned& getMaxNumActors()const;
	void setMaxNumActors(const unsigned& t_num);
	const unsigned& getInitialNumOrganisms()const;


	virtual void init() = 0; // Anything that happens when the simulation begins, like spawning the first characters
	virtual void update() = 0; // Anything that happens every frame, such as spawning x actor on condition y at time z

};

#endif // !SCENARIO_BASE_H