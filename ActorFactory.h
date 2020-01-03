#ifndef ACTOR_FACTORY_H
#define ACTOR_FACTORY_H

#include <memory>
#include <string>
#include <unordered_map>
#include <SFML/System/Vector2.hpp>
#include "Actor_Base.h"
#include "ActorComponent_Base.h"
#include "ActorComponent_Drawable.h"
#include "ActorComponent_Sprite.h"
#include "ActorComponent_Collidable.h"
#include "ActorComponent_Ai.h"
#include "ActorComponent_Text.h"
#include "SharedContext.h"

class Actor_Base;
class ActorComponent_Base;
enum class ActorComponentType;
using ActorPtr = std::unique_ptr<Actor_Base>;
using ActorPresets = std::unordered_map<std::string, ActorPtr>;

class ActorFactory {

	ActorPtr m_actor_wip; // Actor in process of building
	ActorPresets m_actor_presets;
	SharedContext m_context;

public:
	ActorFactory(const SharedContext& t_context);
	ActorPtr create(); // Construction of actor and give ownership of it
	ActorFactory& newActor();
	ActorFactory& setPosition(const sf::Vector2f& t_position);
	ActorFactory& setRotation(const float& t_rotationDeg);
	template <class TComponent, class ...TArgs>ActorFactory& addComponent(TArgs...  t_compArgs);
	ActorFactory& abortConstruction();
	ActorFactory& savePreset(const std::string& t_presetName);
	ActorFactory& newActorFromPreset(const std::string& t_presetName); // The preset must be previously created from the function save preset
	ActorFactory& makeActorCopy(const ActorPtr& t_mold, ActorPtr& t_out_copy);
	ActorFactory& makeActorCopies(const ActorPtr& t_mold, std::vector<ActorPtr>& t_out_copies, unsigned t_numCopies);
	ActorFactory& loadPresetsFromFile(const std::string& t_fileName, const std::string& t_actorPresetIdentifier = "ACTOR_PRESET", const std::string& t_actorComponentIdentifier = "ACTOR_COMPONENT");
	ActorFactory& purge();
};

#endif // !ACTOR_FACTORY_H