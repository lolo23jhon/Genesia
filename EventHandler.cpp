#include <iostream>
#include "EventHandler.h"

#include "PreprocessorDirectves.h"
#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
#include "Engine.h"
#endif

////////////////////////////////////////////////////////////
Binding::Binding(std::unique_ptr<Action> t_action, std::unique_ptr<BoundKeys> t_keys) : m_action{ std::move(t_action) }, m_keys{ std::move(t_keys) }{}

////////////////////////////////////////////////////////////
void Binding::changeKeys(std::unique_ptr<BoundKeys> t_keys) { m_keys = std::move(t_keys); }

////////////////////////////////////////////////////////////
bool Binding::hasKey(const sf::Keyboard::Key& t_key)const { return m_keys->hasKey(t_key); }

////////////////////////////////////////////////////////////
void Binding::addKey(const sf::Keyboard::Key& t_key) { m_keys->addKey(t_key); }

////////////////////////////////////////////////////////////
void Binding::removeKey(const sf::Keyboard::Key& t_key) { m_keys->removeKey(t_key); }

////////////////////////////////////////////////////////////
bool Binding::checkMatch(const KeySet& t_pressedKeys)const { return m_keys->checkMatch(t_pressedKeys); }

////////////////////////////////////////////////////////////
void Binding::execute(const EventInfo& t_info) { m_action->execute(t_info); }

////////////////////////////////////////////////////////////
const ActionTrigger& Binding::getTrigger()const { return m_action->getTrigger(); }


////////////////////////////////////////////////////////////
void Binding::replace(Binding&& t_bind) {
	m_action.reset();
	m_action = std::move(t_bind.m_action);
	m_keys.reset();
	m_keys = std::move(t_bind.m_keys);
}

////////////////////////////////////////////////////////////
EventHandler::EventHandler() : m_active{}, m_bindings{} {}

////////////////////////////////////////////////////////////
void EventHandler::deleteAction(const ActionId& t_id) {
	for (auto& state_it : m_bindings) {
		auto action_it{ state_it.second.find(t_id) };
		if (action_it == state_it.second.end()) { continue; }
		state_it.second.erase(action_it);
		break;
	}
}

////////////////////////////////////////////////////////////
bool EventHandler::executeAction(const ActionId& t_id, const EventInfo& t_info) {
	auto state_it{ m_bindings.find(t_info.m_engineState) };
	if (state_it == m_bindings.cend()) { return false; }
	auto action_it{ state_it->second.find(t_id) };
	if (action_it == state_it->second.cend()) { return false; }
	action_it->second.execute(t_info);
	return true;
}

////////////////////////////////////////////////////////////
bool EventHandler::hasKey(const ActionId& t_id, const sf::Keyboard::Key& t_key)const {
	for (const auto& state_it : m_bindings) {
		auto action_it{ state_it.second.find(t_id) };
		if (action_it == state_it.second.end()) { continue; }
		if (action_it->second.hasKey(t_key)) { return true; }
	}
	return false;
}

////////////////////////////////////////////////////////////
bool EventHandler::addKey(const ActionId& t_id, const sf::Keyboard::Key& t_key) {
	for (auto& state_it : m_bindings) {
		auto action_it{ state_it.second.find(t_id) };
		if (action_it == state_it.second.end()) { continue; }
		action_it->second.addKey(t_key);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////
bool EventHandler::removeKey(const ActionId& t_id, const sf::Keyboard::Key& t_key) {
	for (auto& state_it : m_bindings) {
		auto action_it{ state_it.second.find(t_id) };
		if (action_it == state_it.second.end()) { continue; }
		action_it->second.removeKey(t_key);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////
void EventHandler::addAction(std::unique_ptr<Action> t_action) {
	const auto& state{ t_action->getEngineState() };
	const auto& id{ t_action->getActionId() };

	Binding temp_binding(std::move(t_action), std::make_unique<BoundKeys>());
	auto state_it{ m_bindings.find(state) };
	if (state_it == m_bindings.end()) {

		// Add state, id, and binding
		std::unordered_map<ActionId, Binding> temp_map;
		temp_map.emplace(id, std::move(temp_binding));

		m_bindings.emplace(state, std::move(temp_map));
		return;
	}

	auto action_it{ state_it->second.find(id) };
	if (action_it == state_it->second.end()) {

		// Add id and binding
		state_it->second.emplace(id, std::move(temp_binding));
		return;
	}

	// Replace binding
	action_it->second.replace(std::move(temp_binding));
}

////////////////////////////////////////////////////////////
void EventHandler::handleEvent(const EventInfo& t_info) {
	const auto& state{ t_info.m_engineState };
	const auto& pkeys{ t_info.m_keysCurrentlyPresssed };
	const auto& rkeys{ t_info.m_keysCurrentlyBeingReleased };

	auto state_it{ m_bindings.find(state) };
	if (state_it == m_bindings.end()) { return; }



	for (auto& it : state_it->second) {
		auto& bind{ it.second };
		bool isExecute{ false };
		switch (bind.getTrigger()) {
		case ActionTrigger::ContinousKeyPress:
			isExecute = bind.checkMatch(pkeys); break;
		case ActionTrigger::SingleKeyRelease:
			isExecute = bind.checkMatch(rkeys); break;
		}

		if (isExecute) {
			bind.execute(t_info);
		}
	}
}