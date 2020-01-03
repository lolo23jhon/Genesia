#include <cassert>
#include "ActorFactory.h"
#include "Actor_Base.h"
#include "Utilities.h"

////////////////////////////////////////////////////////////
ActorFactory::ActorFactory(const SharedContext& t_context) : m_context{ t_context }, m_actor_wip{ nullptr } {}

////////////////////////////////////////////////////////////
ActorPtr ActorFactory::create() {
	return std::move(m_actor_wip);
}


////////////////////////////////////////////////////////////
ActorFactory& ActorFactory::newActor() {
	m_actor_wip = std::make_unique<Actor_Base>();
	return *this;
}

////////////////////////////////////////////////////////////
ActorFactory& ActorFactory::setPosition(const sf::Vector2f& t_position) {
	m_actor_wip->setPosition(t_position.x, t_position.y);
	return *this;
}

////////////////////////////////////////////////////////////
ActorFactory& ActorFactory::setRotation(const float& t_rotationDeg) {
	m_actor_wip->setRotation(t_rotationDeg);
}

////////////////////////////////////////////////////////////
template <class TComponent, class ...TArgs>ActorFactory& ActorFactory::addComponent(TArgs...  t_compArgs) {
	m_actor_wip->forceInsertComponent(t_componentType, std::make_unique<TComponent>(m_actor_wip.get(), t_compArgs...));
	return *this;
}

////////////////////////////////////////////////////////////
ActorFactory& ActorFactory::abortConstruction() {
	m_actor_wip.reset();
	return *this;
}

////////////////////////////////////////////////////////////
ActorFactory& ActorFactory::savePreset(const std::string& t_presetName) {
	auto it{ m_actor_presets.find(t_presetName) };
	if (it == m_actor_presets.end()) {
		if (m_actor_wip == nullptr) {
			assert(!"ActorFactory::savePreset: ActorFactory::savePreset: Actor was nullptr!");
			return *this;
		}
		m_actor_presets.emplace(t_presetName, std::move(m_actor_wip));
	}
	else {
		it->second = std::move(m_actor_wip);
	}
	return *this;
}

////////////////////////////////////////////////////////////
ActorFactory& ActorFactory::newActorFromPreset(const std::string& t_presetName) {
	auto it{ m_actor_presets.find(t_presetName) };
	if (it != m_actor_presets.end()) {
		m_actor_wip = std::make_unique<Actor_Base>(m_context, it->second->m_position, it->second->m_rotation);
		for (const auto& comp_it : it->second->m_components) {
			m_actor_wip->insertComponent(comp_it.first, std::move(comp_it.second->clone(m_context)));
		}
	}
	else {
		assert(!"ActorFactory::newActorFromPreset: The tried to create actor from unexistent preset!");
	}
	return *this;
}

////////////////////////////////////////////////////////////
ActorFactory& ActorFactory::makeActorCopy(const ActorPtr& t_mold, ActorPtr& t_out_copy) {
	t_out_copy = std::make_unique<Actor_Base>();
	for (const auto& comp_it : t_mold->m_components) {
		t_out_copy->insertComponent(comp_it.first, std::move(comp_it.second->clone(m_context)));
	}
	return *this;
}

////////////////////////////////////////////////////////////
ActorFactory& ActorFactory::makeActorCopies(const ActorPtr& t_mold, std::vector<ActorPtr>& t_out_copies, unsigned t_numCopies) {
	if (!t_numCopies) { return *this; }
	for (unsigned i{ 0 }; i < t_numCopies; i++) {
		t_out_copies.emplace_back(std::make_unique<Actor_Base>(m_context, t_mold->m_position, t_mold->m_rotation));
		for (const auto& comp_it : t_mold->m_components) {
			t_out_copies.back()->insertComponent(comp_it.first, std::move(comp_it.second->clone(m_context)));
		}
	}
	return *this;
}

////////////////////////////////////////////////////////////
ActorFactory& ActorFactory::purge() {
	m_actor_wip.reset();
	m_actor_presets.clear();
	return *this;
}


////////////////////////////////////////////////////////////
ActorFactory& ActorFactory::loadPresetsFromFile(const std::string& t_fileName, const std::string& t_actorPresetIdentifier, const std::string& t_actorComponentIdentifier) {
	std::string path{ utilities::getWorkingDirectory() };
	std::string fullFileName{ path + t_fileName };
	std::stringstream stream;
	if (!utilities::readFile(fullFileName, stream, true)) {
		assert(!"ActorFactory::loadPresetsFromFile: Failed loading from file!");
		return *this;
	}

	std::string token;
	// Read in as long as there are actors to be read
	while (stream >> token) {
		std::string presetName;

		// Identify the keyword to denote an actor preset
		if (token == t_actorPresetIdentifier) {

			newActor(); // Start object construction


			stream >> presetName;
			unsigned numComponents{ std::stoul(token) }; // Get the number of components

			// Continue as long as there ara actor components to be read
			while (stream >> token) {

				// Check for actor component identifier ("ACTOR_COMPONENET" by defaults)
				if (token == t_actorComponentIdentifier) {

					// Component confirmed, now get the type of component
					auto componentId{ ActorComponent_Base::strToId(token) };

					// Validate the componenet type
					if (componentId == ActorComponentType::INVALID_ACTOR_COMPONENT) {
						std::cout << "! WARNING: Invalid actor component type \"" << token << "\" in file \"" << fullFileName << "\"" << std::endl;
						continue;
					}

					// Createa each component with its special strinsgream constructor
					// (Use lower level ways because we cannot deduce type template parameter from enum class... without creating a black hole)
					m_actor_wip->insertComponent(componentId, std::move(ActorComponent_Base::createComponent(m_context, componentId, stream)));
				}
				else {
					std::cout << "! WARNING: Invalid actor component identifier keyword in file \"" << fullFileName << "\"" << std::endl;
					continue;
				}
			}
		}
		else {
			std::cout << "! WARNING: Invalid actor preset identifier keyword in file \"" << fullFileName << "\"" << std::endl;
			continue;
		}
		savePreset(presetName); // Save the preset preset loaded into the construction test tube
	}
	return *this;
}
