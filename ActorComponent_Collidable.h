#ifndef ACTOR_COMPONENT_COLLIDABLE
#define ACTOR_COMPONENT_COLLIDABLE

#include "ActorComponent_Drawable.h"

// This class defines the hitbos of an actor, theres no need for it to be drawn per see other than for debug purposes
//		or if the actor has no sprite, it can use this as shape model

enum class CollidesWith {
	Nothing

};


class ActorComponent_Collidable : public ActorComponent_Drawable {

	CollidesWith m_collsionSetting;



};


#endif // !ACTOR_COMPONENT_COLLIDABLE
