#ifndef ACTOR_FACTORY_H
#define ACTOR_FACTORY_H

#include <memory>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Actor_Base.h"
#include "ActorComponent_Base.h"
#include "ActorComponent_Drawable.h"
#include "ActorComponent_Sprite.h"
#include "ActorComponent_Collidable.h"
#include "ActorComponent_Ai.h"
#include "ActorComponent_Text.h"

class Actor_Base;
class ActorComponent_Base;
enum class ActorComponentType;




class ActorFactory {

	std::unique_ptr<Actor_Base> m_actor_wip; // Actor in process of building

public:
	ActorFactory();
	std::unique_ptr<Actor_Base> create(); // Construction of actor and give ownership of it
	ActorFactory& newActor();
	ActorFactory& setPosition(const sf::Vector2f& t_position);
	ActorFactory& setRotation(const float& t_rotationDeg);
	template <class TComponent, class ...TArgs>ActorFactory& addComponent(TArgs...  t_compArgs);
	ActorFactory& abortConstruction();
};

#endif // !ACTOR_FACTORY_H
