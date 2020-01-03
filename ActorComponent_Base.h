#ifndef ACTOR_COMPONENT_BASE_H
#define ACTOR_COMPONENT_BASE_H

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <SFML/System/Vector2.hpp>

class Actor_Base;

enum class ActorComponentType {
	INVALID_ACTOR_COMPONENT = -1,
	Collidable,
	Ai,
	Sprite,
	Text
};

using ActorComponentStrings = std::unordered_map<std::string, ActorComponentType>;
using ActorComponentDrawables = std::unordered_set<ActorComponentType>;
class ActorComponent_Base {
	friend class ActorFactory;

	static const ActorComponentStrings s_componentNames;
	static const ActorComponentDrawables s_drawables;

public:
	static bool isComponentDrawable(const ActorComponentType& t_type);
	static const std::string& idToStr(const ActorComponentType& t_compType);
	static ActorComponentType strToId(const std::string& t_compStr);

	virtual void update(Actor_Base* t_owner) = 0; // Update any functionality
	virtual void reset(Actor_Base* t_owner) = 0; // General reset; optional
	virtual void draw(Actor_Base* t_owner) = 0; // Draw on screen command

	virtual ActorComponentPtr clone(SharedContext& t_context)const = 0; // Makes a copy of the actor component and hands it out as a unique ptr
};

#endif // !ACTOR_COMPONENT_BASE_H