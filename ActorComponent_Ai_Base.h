#ifndef ACTOR_COMPONENT_AI_BASE_H
#define ACTOR_COMPONENT_AI_BASE_H

#include "ActorComponent_Base.h"
#include "SharedContext.h"

class ActorComponent_Ai_Base : public ActorComponent_Base {

protected:
	float m_rotationSpeed;
	float m_movementSpeed;

public:
	ActorComponent_Ai_Base(SharedContext& t_context, const float& t_movementSpeed, const float& t_rotationSpeed);
	ActorComponent_Ai_Base(SharedContext& t_context, std::stringstream& t_stream);
	const float& getRotationSpeed()const;
	void setRotationSpeed(const float& t_rotationSpeed);
	const float& getMovementSpeed()const;
	void setMovementSpeed(const float& t_movementSpeed);

	//	The direction is a the delta angle from the actor's rotation in the range [0 360) degrees clockwise
	//	E.g. the value of 0 causes the actor to move paralel to its rotation (by defaults the horizontal axis when it' also at 0)
	virtual void move(Actor_Base* t_owner, const float& t_relativeDirectionDegrees = 0);
	virtual void rotate(Actor_Base* t_owner, bool t_leftRight); // false = left; true = right
};

#endif // !ACTOR_COMPONENT_AI_H