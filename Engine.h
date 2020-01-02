#ifndef  ENGINE_H
#define	 ENGINE_H

#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Keyboard.h"
#include "Actor_Base.h"
#include "EngineTypes.h"
#include "EventHandler.h"
#include "RandomGenerator.h"
#include "ActorFactory.h"

using Actors = std::vector<std::unique_ptr<Actor_Base>>; // contains all the actors in the current simulation
using StateNames = std::map<std::string, EngineState>;
struct EventInfo;



class Engine {
private:
	sf::RenderWindow m_window;
	sf::Text m_guiText;
	sf::View m_view;
	sf::Vector2u m_windowSize;

	EngineState m_state;
	unsigned m_fpsLimit;

	Actors  m_actors;
	ActorFactory m_actorFactory;

	float m_viewSpeed;
	float m_viewZoom;
	unsigned m_maxFramerate;

	sf::Clock m_clock;
	sf::Time m_elapsed;

	Keyboard m_keyboard;

	EventHandler m_eventHandler;
	
	RandomGenerator m_rng;

	static const ActionFactory s_actions;
	static const StateNames s_stateNames; // Map for engine states string names and ids

public:
	Engine(const sf::Vector2u& t_windowSize, const std::string& t_windowName);
	void init();

	// Contains the main loop
	void run();
private:
	bool parseBindings(const std::string& t_fileNameWithPath, const std::string& t_bindingIdentifier = "BIND");

public:
	// View change settings
	float getRandom(const float& t_min, const float& t_max);
	int getRandom(const int& t_min, const int& t_max);
	float getViewChangeSpeed()const;
	void setViewChangeSpeed(const float& t_speed);
	float getZoom()const;
	void setZoom(const float& t_zoom);
	unsigned getMaxFramerate()const;
	void setMaxFramerate(const unsigned& t_fps);
	sf::Time getElapsed()const;
	void resetView();

	sf::RenderWindow& getWindow();
	const EngineState& getState()const;

	void pollEvents();
	void render();
	void update();
	void lateUpdate();

	bool executeAction(const ActionId& t_id, const EventInfo& t_info); // Umbrella for all the actions
	ActionCallback getActionCallback(const EngineState& t_state, const ActionId& t_id); // * See coment bellow

	static ActionId actionStrToId(const std::string& t_name);
	static const std::string& actionIdToStr(const ActionId& t_id);
	static EngineState getStateId(const std::string& t_stateName);
	static const std::string& getStateStr(const EngineState& t_stateId);
	std::unique_ptr<Action> createAction(const ActionId& t_id);


private:
	// Inout actions that can be queued by user inputs (real time actions take in delta time)
	void Action_Pause(const EventInfo& t_info);
	void Action_Unpause(const EventInfo& t_info);
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


//* Hold on... are we searching callbacks by hash (unordered map with callbacks) or switch?
	//	The answer is BOTH: the switch for binding of the the action ids to their fn ptrs in the map at construction
	//		and the hash for general usage. This is to allow for dynamic keybindings:
	//		Lets say we have an action id, which we can exchange for the function ptr and for the key bindings
	//		If we use switch and hash every time (switch for fn ptrs and hash only for bindingd) we're doing a double
	//		lookup on each action. For this we give the hash the functionality of the switch by mapping the action
	//		callbacks even if we're never going to change them. Using only a switch wouldn't allow to change keybindings
	//		dinamically, and using only hash would require a sort of horrible Lovecraftian metaprogram to write functions 
	//		with code somehow which will cause your compiler to speak in tongues on the smallest mistake.

#endif // ! ENGINE_H