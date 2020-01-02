#include "ActorComponent_Base.h"
#include "Actor_Base.h"
#include "MathHelpers.h"

////////////////////////////////////////////////////////////
const ActorComponentStrings ActorComponent_Base::s_componentNames{
	{"Collidable", ActorComponentType::Collidable},
	{"Ai", ActorComponentType::Ai},
	{"Sprite",ActorComponentType::Sprite},
	{"Text",ActorComponentType::Text}
};

static const std::string S_EMPTY_STRING{ "" };

////////////////////////////////////////////////////////////
const std::string& ActorComponent_Base::idToStr(const ActorComponentType& t_compType) {
	auto it{ std::find_if(s_componentNames.cbegin(), s_componentNames.cend(),
		[&t_compType](const std::pair<std::string, ActorComponentType>& t_p) {return t_p.second == t_compType; }) };
	return (it == s_componentNames.cend() ? S_EMPTY_STRING : it->first);
}
////////////////////////////////////////////////////////////
ActorComponentType ActorComponent_Base::strToId(const std::string& t_compStr) {
	auto it{ s_componentNames.find(t_compStr) };
	return (it == s_componentNames.cend() ? ActorComponentType::INVALID_ACTOR_COMPONENT : it->second);
}

////////////////////////////////////////////////////////////
bool ActorComponent_Base::isComponentDrawable(const ActorComponentType& t_componentId) {
	switch (t_componentId) {
	case ActorComponentType::Ai:
		return false;
	case ActorComponentType::Collidable:
	case ActorComponentType::Sprite:
	case ActorComponentType::Text:
	default:
		return true;
	}
}