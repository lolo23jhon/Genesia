#include <cassert>
#include "ActorFactory.h"
#include "Actor_Base.h"

////////////////////////////////////////////////////////////
ActorFactory::ActorFactory(const SharedContext& t_context) : m_context{ t_context }, m_actor_wip{ nullptr } {}

////////////////////////////////////////////////////////////
std::unique_ptr<Actor_Base> ActorFactory::create() {
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
}