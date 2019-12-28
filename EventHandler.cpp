#include <iostream>
#include "EventHandler.h"

#include "PreprocessorDirectves.h"
#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
#include "Engine.h"
#endif


////////////////////////////////////////////////////////////
EventHandler::EventHandler() : m_active{}, m_bindings{} {}

////////////////////////////////////////////////////////////
void EventHandler::deleteCallback(const EngineState& t_state, const ActionId& t_id) {
	auto state_it{ m_bindings.find(t_state) };
	if (state_it == m_bindings.cend()) { return; }
	auto action_it{ state_it->second.find(t_id) };
	if (action_it == state_it->second.cend()) { return; }
	state_it->second.erase(action_it);
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
bool EventHandler::hasKey(const EngineState& t_state, const ActionId& t_id, const sf::Keyboard::Key& t_key) {
	auto state_it{ m_bindings.find(t_state) };
	if (state_it == m_bindings.cend()) { return false; }
	auto action_it{ state_it->second.find(t_id) };
	if (action_it == state_it->second.cend()) { return false; }
	return action_it->second.keys().hasKey(t_key);
}

////////////////////////////////////////////////////////////
bool EventHandler::addKey(const EngineState& t_state, const ActionId& t_id, const sf::Keyboard::Key& t_key) {
	auto state_it{ m_bindings.find(t_state) };
	if (state_it == m_bindings.cend()) { return false; }
	auto action_it{ state_it->second.find(t_id) };
	if (action_it == state_it->second.cend()) { return false; }
	action_it->second.keys().addKey(t_key);
	return true;
}

////////////////////////////////////////////////////////////
bool EventHandler::removeKey(const EngineState& t_state, const ActionId& t_id, const sf::Keyboard::Key& t_key) {
	auto state_it{ m_bindings.find(t_state) };
	if (state_it == m_bindings.cend()) { return false; }
	auto action_it{ state_it->second.find(t_id) };
	if (action_it == state_it->second.cend()) { return false; }
	action_it->second.keys().removeKey(t_key);
	return true;
}

////////////////////////////////////////////////////////////
void EventHandler::addCallback(const EngineState& t_state, const ActionId& t_id, const ActionCallback& t_cb) {
	auto state_it{ m_bindings.find(t_state) };
	if (state_it == m_bindings.cend()) {
		// Register state, action and callback
		
		CallbackBinding temp_callbackBinding{ t_cb };
		std::unordered_map<ActionId, CallbackBinding> temp_subMap;
		temp_subMap.emplace(t_id, std::move(temp_callbackBinding));

		m_bindings.emplace(t_state,std::move(temp_subMap));
		return;
	}
	auto action_it{ state_it->second.find(t_id) };
	if (action_it == state_it->second.cend()) {
		// Register action and callback
		state_it->second.emplace(t_id, std::move(CallbackBinding(t_cb)));
		return;
	}

	// Replace current callback binding
	action_it->second = CallbackBinding(t_cb);
}


////////////////////////////////////////////////////////////
const ActiveActions& EventHandler::getActiveActions(const EngineState& t_state, const PressedKeys& t_pressedKeys) {
	auto state_it{ m_bindings.find(t_state) };
	if (state_it == m_bindings.cend()) {
		// The engine entered an unexistent state... somehow
		m_active.clear();
		return m_active;
	}
	for (auto& action_it : state_it->second) {
		if (action_it.second.keys().checkMatch(t_pressedKeys)) {
			m_active.emplace(action_it.first);
		}
		else {
			m_active.erase(action_it.first);
		}
	}
	return m_active;
}

////////////////////////////////////////////////////////////
void EventHandler::handleEvent(const EventInfo& t_info) {
	getActiveActions(t_info.m_engineState, t_info.m_keysCurrentlyPresssed);

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	size_t counter{ 0 };
#endif

	for (auto& action : m_active) {
		executeAction(action, t_info);

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
		if (!counter) { std::cout << "> ACTIONS\t"; }
		counter++;
		std::cout << Engine::getActionStr(action) << '\t';
		if (counter == m_active.size()) { std::cout << std::endl; }
#endif

	}
}