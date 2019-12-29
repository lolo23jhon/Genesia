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
class Binding;
using Bindings = std::unordered_map<EngineState, std::unordered_map<ActionId, Binding>>;
using ActiveActions = std::unordered_set<ActionId>;


class Binding {
	std::unique_ptr<Action> m_action;
	std::unique_ptr<BoundKeys> m_keys;

public:
	Binding(std::unique_ptr<Action> t_action, std::unique_ptr<BoundKeys> t_keys);
	void changeKeys(std::unique_ptr<BoundKeys> t_keys);
	bool hasKey(const sf::Keyboard::Key& t_key)const;
	void addKey(const sf::Keyboard::Key& t_key);
	void removeKey(const sf::Keyboard::Key& t_key);
	bool checkMatch(const KeySet& t_keys)const;
	const ActionTrigger& getTrigger()const;
	void execute(const EventInfo& t_info);
	void replace(Binding&& t_bind);
};


struct EventInfo {

	const EngineState& m_engineState;
	const KeySet& m_keysCurrentlyPresssed;
	const KeySet& m_keysCurrentlyBeingReleased;
	const float& m_timeElapsedSeconds;

	EventInfo(const EngineState& t_state, const KeySet& t_pressedKeys,const KeySet& t_releasedKeys, const float& t_elapsed) :
		m_engineState{ t_state }, m_keysCurrentlyPresssed{ t_pressedKeys }, m_keysCurrentlyBeingReleased{t_releasedKeys}, m_timeElapsedSeconds{ t_elapsed }{}

};

class EventHandler {
	friend class Engine;

	Bindings m_bindings;
	ActiveActions m_active;

	EventHandler();
	void addAction(std::unique_ptr<Action> t_action);
	const ActiveActions& getActiveActions(const EventInfo& t_info);
	void deleteAction(const ActionId& t_id);
	void handleEvent(const EventInfo& t_info);
	bool executeAction(const ActionId& t_id, const EventInfo& t_info);
	bool hasKey(const ActionId& t_id, const sf::Keyboard::Key& t_key)const;
	bool addKey(const ActionId& t_id, const sf::Keyboard::Key& t_key);
	bool removeKey(const ActionId& t_id, const sf::Keyboard::Key& t_key);

};

#endif // !EVENT_HANDLER_H