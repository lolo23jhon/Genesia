#ifndef AI_ORGANISM_H
#define AI_ORGANISM_H

#include "Ai_Base.h"

class Actor_Base;
struct SharedContext;

class Ai_Organism : public Ai_Base {

	float m_noiseIncrement;

public:
	Ai_Organism(SharedContext& t_context);
	void update(Actor_Base* t_owner, const float& t_elapsed);

};

#endif // !AI_ORGANISM_H