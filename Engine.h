#ifndef  ENGINE_H
#define	 ENGINE_H

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "EventHandler.h"
#include "Actor.h"

using Actors = std::vector<std::unique_ptr<Actor>>; // Contains all the actors in the current simulation

class Engine;
enum class ActionId;
using ActionNames = std::unordered_map<std::string, Engine::ActionId>;

using ActionBinding = std::pair<Engine::ActionId, Binding>;
enum class State;
using ActionBindings = std::unordered_map<ActionId, ActionBnding >;

class Engine {

	// ------------------------------------------------------------------------------------------------------------
	//                                             ENGINE STATES
	// ------------------------------------------------------------------------------------------------------------
public:
	enum class State {
		INIT,
		PAUSED,
		RUNNING
	};


private:
	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize;
	sf::Text m_guiText;
	sf::View m_view;

	State m_state;
	unsigned m_fpsLimit;

	Actors  m_actors;


	sf::Clock m_clock;
	sf::Time m_elapsed;

public:
	Engine(const sf::Vector2u& t_windowSize, const std::string& t_windowName);
	void init();
	void run(); // Contains the main loop


	void render();
	void update();
	void lateUpdate();
	const sf::RenderWindow& getWindow()const;
	sf::Time getElapsed()const;

private:



	void handleInput(sf::Keyboard::Key t_key, bool t_pressed);
	void processEvents(const float& t_dt);
	void handleInput(const sf::Keyboard::Key& t_key, const float& s_dt, bool t_pressed);
	void render();

	// ------------------------------------------------------------------------------------------------------------
	//                                               KEY BINDINGS
	// ------------------------------------------------------------------------------------------------------------

	Keyboard m_keyboard;

	// ------------------------------------------------------------------------------------------------------------
	//                                                 ACTIONS
	// ------------------------------------------------------------------------------------------------------------

public:
	enum class ActionId {
		INVALID_ACTION = -1,
		Pause,
		MoveViewUp,
		MoveViewDown,
		MoveViewLeft,
		MoveViewRight,
		ResetView,
		ZoomIn,
		ZoomOut,
		ResetZoom,
		Save,
		Quit,
		ACTION_COUNT
	};

	static const std::string& getActionStr(const ActionId& t_id);
	static ActionId getActionId(const std::string& t_name);

private:

	static const ActionNames s_actionNames; // Map for action string names and ids

	EventHandler m_eventHandler;

	bool executeAction(const ActionId& t_id,const EventInfo& t_info); // Umbrella for all the actions

	// Inout actions that can be queued by user inputs (real time actions take in delta time)
	void Action_Pause(const EventInfo& t_info);
	void Action_MoveViewLeft(const EventInfo& t_info);
	void Action_MoveViewRight(const EventInfo& t_info);
	void Action_MoveViewUp(const EventInfo& t_info);
	void Action_MoveViewDown(const EventInfo& t_info);
	void Action_ResetView(const EventInfo& t_info);
	void Action_ZoomIn(const EventInfo& t_info);
	void Action_ZoomOut(const EventInfo& t_info);
	void Action_ResetZoom(const EventInfo& t_info);
	void Action_Save(const EventInfo& t_info);
	void Action_Quit(const EventInfo& t_info);
};

#endif // ! ENGINE_H
