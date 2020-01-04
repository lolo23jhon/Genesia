#ifndef ACTOR_COMPONENT_BASE_H
#define ACTOR_COMPONENT_BASE_H

#include <cassert>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <SFML/System/Vector2.hpp>

class Actor_Base;
class ActorComponent_Text;
class ActorComponent_Ai_Basic;
class ActorComponent_Collidable_Circle;
class ActorComponent_Sprite;

enum class ActorComponentType {
	INVALID_ACTOR_COMPONENT = -1,
	Collidable_Circle,
	Ai_Basic,
	Sprite,
	Text
};

using ActorComponentStrings = std::unordered_map<std::string, ActorComponentType>;
using ActorComponentDrawables = std::unordered_set<ActorComponentType>;
class ActorComponent_Base;
using ActorComponentPtr = std::unique_ptr<ActorComponent_Base>;
struct SharedContext;

class ActorComponent_Base {
	
	static const ActorComponentStrings s_componentNames;
	static const ActorComponentDrawables s_drawables;

public:
	static ActorComponentPtr createComponent(const ActorComponentType& t_type, SharedContext& t_context, std::stringstream& t_stream);

	static bool isComponentDrawable(const ActorComponentType& t_type);
	static const std::string& idToStr(const ActorComponentType& t_compType);
	static ActorComponentType strToId(const std::string& t_compStr);

	virtual void update(Actor_Base* t_owner) = 0; // Update any functionality
	virtual void reset(Actor_Base* t_owner) = 0; // General reset; optional
	virtual void draw(Actor_Base* t_owner) = 0; // Draw on screen command

	virtual ActorComponentPtr clone(SharedContext& t_context)const = 0; // Makes a copy of the actor component and hands it out as a unique ptr

	////////////////////////////////////////////////////////////
	template <class ...TArgs> static ActorComponentPtr createComponent(const ActorComponentType& t_type, SharedContext& t_context, TArgs... t_args) {
		switch (t_type) {
		case ActorComponentType::Ai_Basic:
			return std::make_unique<ActorComponent_Ai_Basic>(t_context, t_args...);
		case ActorComponentType::Sprite:
			return std::make_unique<ActorComponent_Sprite>(t_context, t_args...);
		case ActorComponentType::Collidable_Circle:
			return std::make_unique<ActorComponent_Collidable_Circle>(t_context, t_args...);
		case ActorComponentType::Text:
			return std::make_unique<ActorComponent_Text>(t_context, t_args...);
		default:
			assert(!"ActorComponent_Base::createComponent: Invalid component type!");
			return nullptr;
		}
	}
};

#endif // !ACTOR_COMPONENT_BASE_H