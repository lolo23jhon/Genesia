#ifndef ACTOR_COMPONENT_BASE_H
#define ACTOR_COMPONENT_BASE_H

#include <unordered_map>
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

class ActorComponent_Base {

	static const ActorComponentStrings s_componentNames;

public:
	static const std::string& idToStr(const ActorComponentType& t_compType);
	static ActorComponentType strToId(const std::string& t_compStr);


	virtual void update(Actor_Base* t_owner) = 0; // Update any functionality
	virtual void reset(Actor_Base* t_owner)	 = 0; // General reset; optional
	virtual void draw(Actor_Base* t_owner)   = 0; // Draw on screen command

	static bool isComponentDrawable(const ActorComponentType& t_componentId);

};

#endif // !ACTOR_COMPONENT_BASE_H