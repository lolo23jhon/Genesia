#ifndef ACTOR_COMPONENT_AI_BASIC_H
#define ACTOR_COMPONENT_AI_BASIC_H

#include <sstream>
#include "ActorComponent_Ai_Base.h"


struct SharedContext;

class ActorComponent_Ai_Basic : public ActorComponent_Ai_Base {

public:
	ActorComponent_Ai_Basic(SharedContext& t_context, const float& t_movementSpeed, const float& t_rotationSpeed);
	ActorComponent_Ai_Basic(SharedContext& t_context, std::stringstream& t_stream);

};
#endif // !ACTOR_COMPONENT_AI_BASIC_H
