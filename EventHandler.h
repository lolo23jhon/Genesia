#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <functional>
#include <memory>
#include <unordered_map>
#include "Engine.h"
#include "BoundKeys.h"
#include "Keyboard.h"

using Callback = std::function <void(const EventInfo&)>;
using Bindings = std::unordered_map<Engine::ActionId, std::pair< Callback, std::unique_ptr<BoundKeys>>>;
using ActiveActions = std::unordered_set<Engine::ActionId>;

struct EventInfo {
	enum class KeyAction {
		Released = 0,
		Pressed = 1,
	};

	const Engine::State& m_engineState;
	const PressedKeys& m_keysCurrentlyPresssed;
	const float& m_timeElapsedSeconds;
};

class EventHandler {
	friend class Engine;

	Bindings m_bindings;
	ActiveActions m_active;

	////////////////////////////////////////////////////////////
	template <typename T>
	void addCallback(const Engine::ActionId& t_id, T& t_cb, Engine* t_instance) {
		Callback tmp_cb{ std::bind(&t_cb, t_instance,std::placehoders::_1) }; // Hold the place of the EventInfo const reference

		auto it{ m_bindings.find(t_id) };
		if (it == m_bindings.end()) {
			m_bindings.emplace(t_id, { tmp_cb , nullptr });
		}
		else {
			it->swap(tmp_cb);
		}
	}

	const ActiveActions& getActiveActions(const PressedKeys& t_pressedKeys);
	void deleteCallback(const Engine::ActionId& t_id);
	bool handleEvent(const EventInfo& t_info);
	bool executeAction(const Engine::ActionId& t_id, const EventInfo& t_info);
	bool hasKey(const Engine::ActionId& t_id, const sf::Keyboard::Key& t_key);
	bool addKey(const Engine::ActionId& t_id, const sf::Keyboard::Key& t_key);
	bool removeKey(const Engine::ActionId& t_id, const sf::Keyboard::Key& t_key);

};

#endif // !EVENT_HANDLER_H
