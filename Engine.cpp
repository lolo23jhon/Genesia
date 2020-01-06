#include <iostream>
#include "PreprocessorDirectves.h"
#include "Engine.h"
#include "Utilities.h"
#include "SharedContext.h"
#include "Scenario_Basic.h"
#include "Organism.h"

static const sf::Color S_BG_COLOR{ 240,240,240 };

////////////////////////////////////////////////////////////
Engine::Engine(const sf::Vector2u& t_windowSize, const std::string& t_windowName) :
	m_windowSize{ t_windowSize },
	m_keyboard{ Keyboard() },
	m_eventHandler{ EventHandler() },
	m_state{ EngineState::Loading },
	m_window{ sf::VideoMode(t_windowSize.x, t_windowSize.y), t_windowName },
	m_scenario{ nullptr },
	m_rng{},
	m_resourceHolder{},
	m_context{}
{
	m_state = EngineState::Paused;
	m_context.m_engine = this;
	m_context.m_resourceHolder = &m_resourceHolder;
	m_context.m_rng = &m_rng;
	m_context.m_window = &m_window;
	init();
}


////////////////////////////////////////////////////////////
void Engine::init() {
	// Set default view settins
	m_viewSpeed = 10000.f;
	m_viewZoom = 0.05f;
	resetView();

	// Initialize all actions with empty kebindings
	for (const auto& it : s_actions) {
		m_eventHandler.addAction(std::move(createAction(it.first)));
	}

	// Add all the keybindings
	if (!parseBindings("keybindings.txt")) {
		std::cout << "Press Enter to exit.\n";
		std::cin.get();
		std::exit(1);
	}

	// Read in all the resources in the dedicated directory
	m_resourceHolder.init();

	// Initialize simulation scenario
	m_scenario = std::make_unique<Scenario_Basic>(m_context, 100U, 5U);
	m_scenario->init();

}

////////////////////////////////////////////////////////////
void Engine::update() {
	for (auto& actor : m_actors) {
		actor->update();
	}
	m_scenario->update();

}


////////////////////////////////////////////////////////////
const EngineState& Engine::getState()const { return m_state; }


////////////////////////////////////////////////////////////
bool Engine::executeAction(const ActionId& t_id, const EventInfo& t_info) {
	return m_eventHandler.executeAction(t_id, t_info);
}

////////////////////////////////////////////////////////////
sf::Time Engine::getElapsed()const { return m_elapsed; }


////////////////////////////////////////////////////////////
void Engine::run() {

	// Window loop
	while (m_window.isOpen()) {

		// Restar the clock and capture elapsed time
		m_elapsed = m_clock.restart();
		pollEvents();
		update();
		render();
	}
}


////////////////////////////////////////////////////////////
void Engine::pollEvents() {

	m_keyboard.reset();
	const auto& pressedKeys{ m_keyboard.getPressedKeys() };
	const auto& releasedKeys{ m_keyboard.getReleasedKeys() };

	sf::Event e;
	while (m_window.pollEvent(e)) {
		m_keyboard.handleKeyboardInput(e); // Updte keyboard
		//                 #%#
		//               #%%#   
		//				 %#
		//              | |
		//              | |\|\|\|\|\
		// sf::Event -> |   __      | -> EventInfo
		//				|__|  |_____|
		EventInfo eventInfo(m_state, pressedKeys, releasedKeys, m_elapsed.asSeconds());
		m_eventHandler.handleEvent(eventInfo);

		switch (e.type) {

		case sf::Event::Resized:
			m_window.setSize(m_windowSize);
			break;

		case sf::Event::Closed:
			m_window.close();
			break;
		}
	}

}

////////////////////////////////////////////////////////////
float Engine::getRandom(const float& t_min, const float& t_max) { return m_rng(t_min, t_max); }

////////////////////////////////////////////////////////////
int Engine::getRandom(const int& t_min, const int& t_max) { return m_rng(t_min, t_max); }

////////////////////////////////////////////////////////////
sf::RenderWindow& Engine::getWindow() {
	return m_window;
}


////////////////////////////////////////////////////////////
void Engine::render() {

	// Apply the engine view to the window
	m_window.setView(m_view);

	// Clear the window
	m_window.clear(S_BG_COLOR);

	// Draw the actors
	for (auto& actor : m_actors) {
		actor->draw();
	}

	m_window.display();
}

////////////////////////////////////////////////////////////
bool Engine::parseBindings(const std::string& t_fileNameWithPath, const std::string& t_bindingIdentifier) {
	std::stringstream stream;

	if (!utilities::readFile(t_fileNameWithPath, stream, false)) {
		std::cerr << "@ ERROR: Cannot parse keyboard bindings file \"" << t_fileNameWithPath << '\"' << std::endl;
		return false;
	}

	std::string token;
	while (stream >> token) {

		if (token == t_bindingIdentifier) {
			std::string action;
			std::string key;

			stream >> action >> key;

			auto actionId{ actionStrToId(action) };
			if (actionId == ActionId::INVALID_ACTION) {
				std::cerr << "! WARNING: Invalid action \"" << action << "\" in file \"" << t_fileNameWithPath << '\"' << std::endl;
				continue;
			} // Invalid action; try to read a new binding

			auto keyId{ Keyboard::getKeyId(key) };
			if (m_keyboard.getKeyId(key) == sf::Keyboard::Unknown) {
				std::cout << "! WARNING: Invalid key \"" << key << "\" in file \"" << t_fileNameWithPath << '\"' << std::endl;
				continue;
			}// Invalid key; try to read a new binding

			if (!m_eventHandler.addKey(actionId, keyId)) {
				std::cerr << "! WARNING: Failed to add key \"" << key << "\" to action  \"" << action << '\"' << std::endl;
			}


			m_keyboard.listenToKey(keyId); // Tell the keyboard to listen to the key
		}
		else { continue; }// The token wasn't the line identifier; skip it
	}

	return true;
}

////////////////////////////////////////////////////////////
float Engine::getViewChangeSpeed()const { return m_viewSpeed; }

////////////////////////////////////////////////////////////
void  Engine::setViewChangeSpeed(const float& t_speed) { m_viewSpeed = t_speed; }

////////////////////////////////////////////////////////////
float Engine::getZoom()const { return m_viewZoom; }

////////////////////////////////////////////////////////////
void  Engine::setZoom(const float& t_zoom) { m_viewZoom = t_zoom; }

////////////////////////////////////////////////////////////
unsigned Engine::getMaxFramerate()const { return m_maxFramerate; }

////////////////////////////////////////////////////////////
void Engine::setMaxFramerate(const unsigned& t_fps) { m_maxFramerate = t_fps; }

////////////////////////////////////////////////////////////
void Engine::resetView() {
	m_view = sf::View();
	m_view.setCenter(static_cast<float>(m_windowSize.x) / 2, static_cast<float>(m_windowSize.y) / 2);
}


////////////////////////////////////////////////////////////
void Engine::spawnActor(ActorPtr t_actor) { m_actors.emplace_back(std::move(t_actor)); }


static const std::string S_EMPTY_STR{ "" };

const StateNames Engine::s_stateNames{
	{"EngineState_Loading", EngineState::Loading},
	{"EngineState_Paused", EngineState::Paused},
	{"EngineState_Running", EngineState::Running}
};

//////////////////////////////////////////////////////////
const std::string& Engine::getStateStr(const EngineState& t_stateId) {
	auto it{ std::find_if(s_stateNames.cbegin(), s_stateNames.cend(),
		[&t_stateId](const std::pair<std::string, EngineState> t_p) {return t_p.second == t_stateId; }) };
	return (it == s_stateNames.cend() ? S_EMPTY_STR : it->first);
}

//////////////////////////////////////////////////////////
EngineState Engine::getStateId(const std::string& t_stateName) {
	auto it{ s_stateNames.find(t_stateName) };
	return (it == s_stateNames.cend() ? EngineState::INVALID_STATE : it->second);
}


//////////////////////////////////////////////////////////
const ActionFactory Engine::s_actions{
		{ActionId::Unpause,					{"Action_Unpause",				EngineState::Paused,	ActionTrigger::SingleKeyRelease,	&Engine::Action_Unpause}},
		{ActionId::Save,					{"Action_Save",					EngineState::Paused,	ActionTrigger::SingleKeyRelease,	&Engine::Action_Save}},
		{ActionId::Quit,					{"Action_Quit",					EngineState::Paused,	ActionTrigger::SingleKeyRelease,	&Engine::Action_Quit}},
		{ActionId::Pause,					{"Action_Pause",				EngineState::Running,	ActionTrigger::SingleKeyRelease,	&Engine::Action_Pause}},
		{ActionId::MoveViewUp,				{"Action_MoveViewUp",			EngineState::Running,	ActionTrigger::ContinousKeyPress,	&Engine::Action_MoveViewUp}},
		{ActionId::MoveViewUp_Paused,		{"Action_MoveViewUp_Paused",	EngineState::Paused,	ActionTrigger::ContinousKeyPress,	&Engine::Action_MoveViewUp_Paused}},
		{ActionId::MoveViewDown,			{"Action_MoveViewDown",			EngineState::Running,	ActionTrigger::ContinousKeyPress,	&Engine::Action_MoveViewDown}},
		{ActionId::MoveViewDown_Paused,		{"Action_MoveViewDown_Paused",	EngineState::Paused,	ActionTrigger::ContinousKeyPress,	&Engine::Action_MoveViewDown_Paused}},
		{ActionId::MoveViewLeft,			{"Action_MoveViewLeft",			EngineState::Running,	ActionTrigger::ContinousKeyPress,	&Engine::Action_MoveViewLeft}},
		{ActionId::MoveViewLeft_Paused,		{"Action_MoveViewLeft_Paused",	EngineState::Paused,	ActionTrigger::ContinousKeyPress,	&Engine::Action_MoveViewLeft_Paused}},
		{ActionId::MoveViewRight,			{"Action_MoveViewRight",		EngineState::Running,	ActionTrigger::ContinousKeyPress,	&Engine::Action_MoveViewRight}},
		{ActionId::MoveViewRight_Paused,	{"Action_MoveViewRight_Paused",	EngineState::Paused,	ActionTrigger::ContinousKeyPress,	&Engine::Action_MoveViewRight_Paused}},
		{ActionId::ResetView,				{"Action_ResetView",			EngineState::Running,	ActionTrigger::SingleKeyRelease,	&Engine::Action_ResetView}},
		{ActionId::ResetView_Paused,		{"Action_ResetView_Paused",		EngineState::Paused,	ActionTrigger::SingleKeyRelease,	&Engine::Action_ResetView_Paused}},
		{ActionId::ZoomIn,					{"Action_ZoomIn",				EngineState::Running,	ActionTrigger::ContinousKeyPress,	&Engine::Action_ZoomIn}},
		{ActionId::ZoomIn_Paused,			{"Action_ZoomIn_Paused",		EngineState::Paused,	ActionTrigger::ContinousKeyPress,	&Engine::Action_ZoomIn_Paused}},
		{ActionId::ZoomOut,					{"Action_ZoomOut",				EngineState::Running,	ActionTrigger::ContinousKeyPress,	&Engine::Action_ZoomOut}},
		{ActionId::ZoomOut_Paused,			{"Action_ZoomOut_Paused",		EngineState::Paused,	ActionTrigger::ContinousKeyPress,	&Engine::Action_ZoomOut_Paused}},
		{ActionId::ResetZoom,				{"Action_ResetZoom",			EngineState::Running,	ActionTrigger::SingleKeyRelease,	&Engine::Action_ResetZoom}},
		{ActionId::ResetZoom_Paused,		{"Action_ResetZoom_Paused",		EngineState::Paused,	ActionTrigger::SingleKeyRelease,	&Engine::Action_ResetZoom_Paused}}
};

////////////////////////////////////////////////////////////
const std::string& Engine::actionIdToStr(const ActionId& t_id) {
	auto it{ s_actions.find(t_id) };
	return (it == s_actions.cend() ? S_EMPTY_STR : std::get<ACTION_NAME>(it->second));
}

////////////////////////////////////////////////////////////
ActionId Engine::actionStrToId(const std::string& t_name) {
	auto it{ std::find_if(s_actions.cbegin(), s_actions.cend(),
		[t_name](const std::pair<ActionId,ActionTuple>& t_p) {return std::get<ACTION_NAME>(t_p.second) == t_name; }) };
	return (it == s_actions.cend() ? ActionId::INVALID_ACTION : it->first);
}

////////////////////////////////////////////////////////////
std::unique_ptr<Action> Engine::createAction(const ActionId& t_id) {
	auto it{ s_actions.find(t_id) };
	if (it == s_actions.cend()) { return nullptr; }

	return std::make_unique<Action>(t_id,
		std::get<ACTION_ENGINE_STATE>(it->second),
		std::get<ACTION_NAME>(it->second),
		std::get<ACTION_TRIGGER>(it->second),
		std::bind(std::get<ACTION_FUNCTOR>(it->second), this, std::placeholders::_1));
}


// --------------------------------------------------------------------------------------------------- ACTIONS

////////////////////////////////////////////////////////////
void Engine::Action_Pause(const EventInfo& t_info) {
	m_state = EngineState::Paused;
#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tPause" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_Unpause(const EventInfo& t_info) {
	m_state = EngineState::Running;
#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tUnpause" << std::endl;
#endif

}

////////////////////////////////////////////////////////////
void Engine::Action_MoveViewLeft(const EventInfo& t_info) {
	m_view.move(-m_viewSpeed * getElapsed().asSeconds(), 0.f);

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tMoveViewLeft" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_MoveViewLeft_Paused(const EventInfo& t_info) {
	m_view.move(-m_viewSpeed * getElapsed().asSeconds(), 0.f);

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tMoveViewLeft_Paused" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_MoveViewRight(const EventInfo& t_info) {
	m_view.move(m_viewSpeed * getElapsed().asSeconds(), 0.f);

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tMoveViewRight" << std::endl;
#endif
}


////////////////////////////////////////////////////////////
void Engine::Action_MoveViewRight_Paused(const EventInfo& t_info) {
	m_view.move(m_viewSpeed * getElapsed().asSeconds(), 0.f);

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tMoveViewRight_Paused" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_MoveViewUp(const EventInfo& t_info) {
	m_view.move(0.f, -m_viewSpeed * getElapsed().asSeconds()); // -y = up

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tMoveViewUp" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_MoveViewUp_Paused(const EventInfo& t_info) {
	m_view.move(0.f, -m_viewSpeed * getElapsed().asSeconds()); // -y = up

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tMoveViewUp_Paused" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_MoveViewDown(const EventInfo& t_info) {
	m_view.move(0.f, m_viewSpeed * getElapsed().asSeconds()); // +y = down

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tMoveViewDown" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_MoveViewDown_Paused(const EventInfo& t_info) {
	m_view.move(0.f, m_viewSpeed * getElapsed().asSeconds()); // +y = down

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tMoveViewDown_Paused" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_ResetView(const EventInfo& t_info) {
	m_view = sf::View();
	m_view.setCenter(static_cast<float>(m_windowSize.x) / 2, static_cast<float>(m_windowSize.y) / 2);

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tResetView" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_ResetView_Paused(const EventInfo& t_info) {
	m_view = sf::View();
	m_view.setCenter(static_cast<float>(m_windowSize.x) / 2, static_cast<float>(m_windowSize.y) / 2);

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tResetView_Paused" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_ZoomIn(const EventInfo& t_info) {
	m_view.zoom(1.f - m_viewZoom);

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tZoomIn" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_ZoomIn_Paused(const EventInfo& t_info) {
	m_view.zoom(1.f - m_viewZoom);

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tZoomIn_Paused" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_ZoomOut(const EventInfo& t_info) {
	m_view.zoom(1.f + m_viewZoom);

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tZoomOut" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_ZoomOut_Paused(const EventInfo& t_info) {
	m_view.zoom(1.f + m_viewZoom);

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tZoomOut_Paused" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_ResetZoom(const EventInfo& t_info) {
	resetView();

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tResetZoom" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_ResetZoom_Paused(const EventInfo& t_info) {
	resetView();

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tResetZoom_Paused" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_Save(const EventInfo& t_info) {

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tSave" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_Quit(const EventInfo& t_info) {

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tQuit" << std::endl;
#endif
}

////////////////////////////////////////////////////////////
void Engine::Action_INVALID_ACTION(const EventInfo& t_info) {
#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 1
	std::cout << "> ACTION\tINVALID_ACTION" << std::endl;
#endif
}

// --------------------------------------------------------------------------------------------------- ACTIONS