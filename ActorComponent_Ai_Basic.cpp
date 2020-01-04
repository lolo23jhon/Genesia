#include "ActorComponent_Ai_Basic.h"

////////////////////////////////////////////////////////////
ActorComponent_Ai_Basic::ActorComponent_Ai_Basic(SharedContext& t_context, const float& t_movementSpeed, const float& t_rotationSpeed) :
	ActorComponent_Ai_Base(t_context, t_movementSpeed, t_rotationSpeed) {}

////////////////////////////////////////////////////////////
ActorComponent_Ai_Basic::ActorComponent_Ai_Basic(SharedContext& t_context, std::stringstream& t_stream) : ActorComponent_Ai_Base(t_context, t_stream) {}