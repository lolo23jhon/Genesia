#include "EventHandler.h"

////////////////////////////////////////////////////////////
void EventHandler::deleteCallback(const Engine::ActionId& t_id) {
	auto it{ m_bindings.find(t_id) };
	if (it == m_bindings.end()) {
		m_bindings.erase(it);
	}
}

////////////////////////////////////////////////////////////
bool EventHandler::executeAction(const Engine::ActionId& t_id, const EventInfo& t_info) {
	auto it{ m_bindings.find(t_id) };
	if (it == m_bindings.end()) { return false; }
	it->second(t_info); // Call parens operator on the functor
	return true;
}

////////////////////////////////////////////////////////////
bool EventHandler::hasKey(const Engine::ActionId& t_id, const sf::Keyboard::Key& t_key) {
	auto it{ m_bindings.find(t_id) };
	if (it == m_bindings.end()) { return false; }
	return true;
}

////////////////////////////////////////////////////////////
bool EventHandler::hasKey(const Engine::ActionId& t_id, const sf::Keyboard::Key& t_key) {
	auto id_it{ m_bindings.find(t_id) };
	if (id_it == m_bindings.end()) { return false; }	// No action id found
	auto& key_ptr{ id_it->second.second };
	if (!key_ptr) { return false; }		// ptr is null
	return key_ptr->hasKey(t_key);
}

////////////////////////////////////////////////////////////
bool EventHandler::addKey(const Engine::ActionId& t_id, const sf::Keyboard::Key& t_key) {
	auto id_it{ m_bindings.find(t_id) };
	if (id_it == m_bindings.end()) { return false; }	// No action id found; cannot try to build binding since we don't have callbacks
	auto& key_ptr{ id_it->second.second };
	if (!key_ptr) {
		key_ptr = std::make_unique<BoundKeys>();  // Make new BoundKeys object
	}

	key_ptr->addKey(t_key);
	return true;
}

////////////////////////////////////////////////////////////
bool EventHandler::removeKey(const Engine::ActionId& t_id, const sf::Keyboard::Key& t_key) {
	auto id_it{ m_bindings.find(t_id) };
	if (id_it == m_bindings.end()) { return false; }
	auto& key_ptr{ id_it->second.second };
	if (!key_ptr) { key_ptr = std::make_unique<BoundKeys>(); }
	key_ptr->removeKey(t_key);
	return true;
}




////////////////////////////////////////////////////////////
const ActiveActions& EventHandler::getActiveActions(const PressedKeys& t_pressedKeys) {
	for (const auto& bind : m_bindings) {
		auto& keys{ bind.second.second };
		if (!keys) { continue; }
		if (keys->checkMatch(t_pressedKeys)) { m_active.emplace(bind.first); } // Match found, add to active envent list
		else { m_active.erase(bind.first); } // No match found, erase the item (if present)
	}
	return m_active;
}


////////////////////////////////////////////////////////////
bool EventHandler::handleEvent(const EventInfo& t_info) {
	getActiveActions(t_info.m_keysCurrentlyPresssed);
	for (auto& action : m_active) {
		executeAction(action, t_info);
	}
}