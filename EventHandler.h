#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <functional>
#include <memory>
#include <unordered_map>
#include "BoundKeys.h"
#include "EngineTypes.h"

class Engine;

struct EventInfo;
using ActionCallback = std::function<void(const EventInfo&)>;
using Bindings = std::unordered_map<ActionId, std::pair<ActionCallback, std::unique_ptr<BoundKeys>>>;
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

	EventHandler();
	void addCallback(const ActionId& t_id, ActionCallback t_cb);
	const ActiveActions& getActiveActions(const PressedKeys& t_pressedKeys);
	void deleteCallback(const ActionId& t_id);
	void handleEvent(const EventInfo& t_info);
	bool executeAction(const ActionId& t_id, const EventInfo& t_info);
	bool hasKey(const ActionId& t_id, const sf::Keyboard::Key& t_key);
	bool addKey(const ActionId& t_id, const sf::Keyboard::Key& t_key);
	bool removeKey(const ActionId& t_id, const sf::Keyboard::Key& t_key);

};

#endif // !EVENT_HANDLER_H