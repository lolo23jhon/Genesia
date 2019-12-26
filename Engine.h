#ifndef  ENGINE_H
#define	 ENGINE_H

#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Keyboard.h"
#include "Actor.h"
#include "EngineTypes.h"
#include "EventHandler.h"

using Actors = std::vector<std::unique_ptr<Actor>>; // contains all the actors in the current simulation
using ActionNames = std::unordered_map<std::string, ActionId>;
struct EventInfo;
class Engine;
using ActionFunctor = void (Engine::*)(const EventInfo&);
using ActionCallback = std::function<void(const EventInfo&)>;


class Engine {
private:
	sf::RenderWindow m_window;
	sf::Text m_guiText;
	sf::View m_view;

	EngineState m_state;
	unsigned m_fpsLimit;

	Actors  m_actors;


	sf::Clock m_clock;
	sf::Time m_elapsed;

	Keyboard m_keyboard;

	EventHandler m_eventHandler;

	static const ActionNames s_actionNames; // Map for action string names and ids

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

	// ------------------------------------------------------------------------------------------------------------
	//                                                 ACTIONS
	// ------------------------------------------------------------------------------------------------------------

public:


	static const std::string& getActionStr(const ActionId& t_id);
	static ActionId getActionId(const std::string& t_name);

private:



	bool parseBindings(const std::string& t_fileNameWithPath, const std::string& t_bindingIdentifier = "BIND");
	bool executeAction(const ActionId& t_id, const EventInfo& t_info); // Umbrella for all the actions
	ActionCallback getActionCallback(const ActionId& t_id);
	// Hold on... are we searching callbacks by hash (unordered map with callbacks) or switch?
	//	The answer is BOTH: the switch for binding of the the action ids to their fn ptrs in the map at construction
	//		and the hash for general usage. This is to allow for dynamic keybindings:
	//		Lets say we have an action id, which we can exchange for the function ptr and for the key bindings
	//		If we use switch and hash every time (switch for fn ptrs and hash only for bindingd) we're doing a double
	//		lookup on each action. For this we give the hash the functionality of the switch by mapping the action
	//		callbacks even if we're never going to change them. Using only a switch wouldn't allow to change keybindings
	//		dinamically, and using only hash would require a sort of horrible Lovecraftian metaprogram to write functions 
	//		with code somehow which will cause your compiler to speak in tongues on the smallest mistake.


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
	void Action_INVALID_ACTION(const EventInfo& t_info);
};

#endif // ! ENGINE_H