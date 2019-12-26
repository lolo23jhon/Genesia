#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <functional>
#include <memory>
#include <unordered_map>
#include "Engine.h"
#include "BoundKeys.h"
#include "Keyboard.h"

using Bindings = std::unordered_map<ActionId, std::pair< ActionFn, std::unique_ptr<BoundKeys>>>;
using ActiveActions = std::unordered_set<ActionId>;

struct EventInfo {

	const EngineState& m_engineState;
	const PressedKeys& m_keysCurrentlyPresssed;
	const float& m_timeElapsedSeconds;

	EventInfo(const EngineState& t_state, const PressedKeys& t_pressedKeys, const float& t_elapsed) :
		m_engineState{ t_state }, m_keysCurrentlyPresssed{ t_pressedKeys }, m_timeElapsedSeconds{ t_elapsed }{}

};

class EventHandler {
	friend class Engine;

	Bindings m_bindings;
	ActiveActions m_active;

	////////////////////////////////////////////////////////////
	void addCallback(const ActionId& t_id, ActionFn& t_cb, Engine* t_instance) {

		auto it{ m_bindings.find(t_id) };
		if (it == m_bindings.end()) {
			m_bindings.emplace(t_id, { t_cb , nullptr }); // Add the callback with no keybindings (yet)
		}
		else {
			it->second = {t_cb, nullptr}; // If action was already bound to id (somehow...), replace it
		}
	}

	const ActiveActions& getActiveActions(const PressedKeys& t_pressedKeys);
	void deleteCallback(const ActionId& t_id);
	bool handleEvent(const EventInfo& t_info);
	bool executeAction(const ActionId& t_id, const EventInfo& t_info);
	bool hasKey(const ActionId& t_id, const sf::Keyboard::Key& t_key);
	bool addKey(const ActionId& t_id, const sf::Keyboard::Key& t_key);
	bool removeKey(const ActionId& t_id, const sf::Keyboard::Key& t_key);

};

#endif // !EVENT_HANDLER_H
