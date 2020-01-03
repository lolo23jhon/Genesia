#include <cassert>
#include "ActorComponent_Base.h"
#include "Actor_Base.h"
#include "MathHelpers.h"
#include "ActorComponent_Ai.h"
#include "ActorComponent_Sprite.h";
#include "ActorComponent_Collidable.h"
#include "ActorComponent_Text.h"


////////////////////////////////////////////////////////////
const ActorComponentStrings ActorComponent_Base::s_componentNames{
	{"Collidable", ActorComponentType::Collidable},
	{"Ai", ActorComponentType::Ai},
	{"Sprite",ActorComponentType::Sprite},
	{"Text",ActorComponentType::Text}
};

////////////////////////////////////////////////////////////
ActorComponentPtr ActorComponent_Base::createComponent(SharedContext& t_context, const ActorComponentType& t_type, std::stringstream& t_stream) {
	switch (t_type) {
	case ActorComponentType::Ai:
		return std::make_unique<ActorComponent_Ai>(t_context, t_stream);
	case ActorComponentType::Sprite:
		return std::make_unique<ActorComponent_Sprite>(t_context, t_stream);
	case ActorComponentType::Collidable:
		return std::make_unique<ActorComponent_Collidable>(t_context, t_stream);
	case ActorComponentType::Text:
		return std::make_unique<ActorComponent_Text>(t_context, t_stream);
	default:
		assert(!"ActorComponent_Base::createComponent: Invalid component type!");
		return nullptr;
	}
}


////////////////////////////////////////////////////////////
const ActorComponentDrawables ActorComponent_Base::s_drawables{
	ActorComponentType::Collidable,
	ActorComponentType::Sprite,
	ActorComponentType::Text
};

////////////////////////////////////////////////////////////
bool ActorComponent_Base::isComponentDrawable(const ActorComponentType& t_type) {
	return (s_drawables.find(t_type) != s_drawables.cend());
}


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