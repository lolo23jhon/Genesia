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
class CallbackBinding;
using Bindings = std::unordered_map<EngineState, std::unordered_map<ActionId, CallbackBinding>>;
using ActiveActions = std::unordered_set<ActionId>;


class CallbackBinding {
	ActionCallback m_callback;
	std::unique_ptr<BoundKeys> m_keys;

public:
	CallbackBinding(const ActionCallback& t_cb, BoundKeys&& t_keys) : m_callback{ t_cb }, m_keys{ std::make_unique<BoundKeys>(t_keys) }{}
	CallbackBinding(const ActionCallback& t_cb) : m_callback{ t_cb }, m_keys{ std::make_unique<BoundKeys>() }{}
	void changeKeys(std::unique_ptr<BoundKeys> t_keys) { m_keys = std::move(t_keys); }
	void execute(const EventInfo& t_info) { m_callback(t_info); }
	BoundKeys& keys() { return *m_keys; }
};


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
	void addCallback(const EngineState& r_state, const ActionId& t_id, const ActionCallback& t_cb);
	const ActiveActions& getActiveActions(const EngineState& t_state, const PressedKeys& t_pressedKeys);
	void deleteCallback(const EngineState& t_state, const ActionId& t_id);
	void handleEvent(const EventInfo& t_info);
	bool executeAction(const ActionId& t_id, const EventInfo& t_info);
	bool hasKey(const EngineState& t_state, const ActionId& t_id, const sf::Keyboard::Key& t_key);
	bool addKey(const EngineState& t_state, const ActionId& t_id, const sf::Keyboard::Key& t_key);
	bool removeKey(const EngineState& t_state, const ActionId& t_id, const sf::Keyboard::Key& t_key);

};

#endif // !EVENT_HANDLER_H