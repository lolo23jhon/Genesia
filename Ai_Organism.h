#ifndef AI_ORGANISM_H
#define AI_ORGANISM_H

#include "Ai._Base.h"

// To createa a credible movement pattern, the organism's determine their 
//  idle rotation on perlin noise

class Ai_Organism : public Ai_Base {
public:
	Ai_Organism();

protected:
	void Task_Idle(Actor_Base* t_owner);
};

#endif // !AI_ORGANISM_H