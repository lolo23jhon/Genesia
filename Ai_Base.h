#ifndef AI_BASE_H
#define AI_BASE_H

class Actor_Base;

class Ai_Base {

public:
	virtual void update(Actor_Base* t_owner, const float& t_elapsed) = 0;

};

#endif // !AI_BASE_H