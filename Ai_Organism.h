#ifndef AI_ORGANISM_H
#define AI_ORGANISM_H

#include "Ai._Base.h"

// To createa a credible movement pattern, the organism's determine their 
//  idle rotation on perlin noise

class Ai_Organism : public Ai_Base {
public:
	Ai_Organism(Actor_Base* t_owner);
	void update(Actor_Base* t_owner, const float& t_elapsed);

protected:
	float m_noiseIncrement; // Random value calculated at contstruction used to create a distinct perlin noise siganture for every organism instance

	void Task_Idle(Actor_Base* t_owner, const float& t_elapsed);
	void Task_Reproduce(Actor_Base* t_owner, const float& t_elapsed);
};

#endif // !AI_ORGANISM_H