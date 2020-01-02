#include "ActorComponent_Base.h"
#include "Actor_Base.h"
#include "MathHelpers.h"

////////////////////////////////////////////////////////////
bool ActorComponent_Base::isComponentDrawable(const ActorComponentType& t_componentId) {
	switch (t_componentId) {
	case ActorComponentType::Ai:
		return false;
	case ActorComponentType::Box:
	case ActorComponentType::Sprite:
	case ActorComponentType::Text:
	default:
		return true;
	}
}