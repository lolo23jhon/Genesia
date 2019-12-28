#include "PreprocessorDirectves.h"
#include "Utilities.h"
#include "Engine.h"


////////////////////////////////////////////////////////////
Engine::Engine(const sf::Vector2u& t_windowSize, const std::string& t_windowName) :
	m_keyboard{ Keyboard() },
	m_eventHandler{ EventHandler() },
	m_state{ EngineState::Loading },
	m_window{ sf::VideoMode(t_windowSize.x, t_windowSize.y), t_windowName }
{

	init();
	m_state = EngineState::Paused;
}


////////////////////////////////////////////////////////////
void Engine::init() {
	m_viewSpeed = 250.f;
	m_viewZoom = 0.05f;
	resetView();

	for (const auto& state_it : s_stateNames) {
		if (state_it.second == EngineState::Loading) { continue; }
		for (const auto& action_it : s_actionNames) {
			m_eventHandler.addCallback(state_it.second, action_it.second, getActionCallback(state_it.second, action_it.second));
		}
	}

	if (!parseBindings("keybindings.txt")) {
		std::cout << "Press Enter to exit.\n";
		std::cin.get();
		std::exit(1);
	}

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

	}
}


////////////////////////////////////////////////////////////
void Engine::pollEvents() {
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
		auto pressedKeys{ m_keyboard.getPressedKeys() };
		EventInfo eventInfo(m_state, pressedKeys, m_elapsed.asSeconds());
		m_eventHandler.handleEvent(eventInfo);

		switch (e.type) {

		case sf::Event::Closed:
			m_window.close();
			break;
		}
	}
}


////////////////////////////////////////////////////////////
const sf::RenderWindow& Engine::getWindow()const {
	return m_window;
}


////////////////////////////////////////////////////////////
void Engine::render() {

	// Apply the engine view to the window
	m_window.setView(m_view);

	// Clear the window
	m_window.clear();
	m_window.setView(m_window.getDefaultView());

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

			auto actionId{ getActionId(action) };
			if (actionId == ActionId::INVALID_ACTION) {
				std::cerr << "! WARNING: Invalid action \"" << action << "\" in file \"" << t_fileNameWithPath << '\"' << std::endl;
				continue;
			} // Invalid action; try to read a new binding

			auto keyId{ Keyboard::getKeyId(key) };
			if (m_keyboard.getKeyId(key) == sf::Keyboard::Unknown) {
				std::cout << "! WARNING: Invalid key \"" << key << "\" in file \"" << t_fileNameWithPath << '\"' << std::endl;
				continue;
			}// Invalid key; try to read a new binding

			for (unsigned i{ 1 }; i < static_cast<unsigned>(EngineState::STATE_COUNT); i++) {
				if (!m_eventHandler.addKey(static_cast<EngineState>(i), actionId, keyId)) {
					std::cerr << "! WARNING: Failed to add key \"" << key << "\" to action  \"" << action << '\"' << std::endl;
				}
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
	m_view.setSize(static_cast<float>(m_windowSize.x), static_cast<float>(m_windowSize.y));
}


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
const EngineState Engine::getStateId(const std::string& t_stateName) {
	auto it{ s_stateNames.find(t_stateName) };
	return (it == s_stateNames.cend() ? EngineState::INVALID_STATE : it->second);
}


//////////////////////////////////////////////////////////
const ActionNames Engine::s_actionNames{
	{"Pause",			ActionId::Pause},
	{"Unpause",			ActionId::Unpause},
	{"MoveViewUp",		ActionId::MoveViewUp},
	{"MoveViewDown",	ActionId::MoveViewDown},
	{"MoveViewLeft",	ActionId::MoveViewLeft},
	{"MoveViewRight",	ActionId::MoveViewRight},
	{"ResetView",		ActionId::ResetView},
	{"ZoomIn",			ActionId::ZoomIn},
	{"ZoomOut",			ActionId::ZoomOut},
	{"ResetZoom",		ActionId::ResetZoom},
	{"Save",			ActionId::Save},
	{"Quit",			ActionId::Quit}
};


////////////////////////////////////////////////////////////
const std::string& Engine::getActionStr(const ActionId& t_id) {
	auto it{ std::find_if(s_actionNames.cbegin(),s_actionNames.cend(),
	[&t_id](const std::pair<std::string, ActionId>& t_p) {return t_p.second == t_id; }) };
	return (it == s_actionNames.cend() ? S_EMPTY_STR : it->first);
}

////////////////////////////////////////////////////////////
ActionId Engine::getActionId(const std::string& t_name) {
	auto it{ s_actionNames.find(t_name) };
	return (it == s_actionNames.cend() ? ActionId::INVALID_ACTION : it->second);
}

////////////////////////////////////////////////////////////
ActionCallback Engine::getActionCallback(const EngineState& t_state, const ActionId& t_id) {
	ActionFunctor functorPtr;
	switch (t_state) {

	case EngineState::Running:

		switch (t_id) {
		case ActionId::Pause:
			functorPtr = &Engine::Action_Pause; break;
		case ActionId::MoveViewUp:
			functorPtr = &Engine::Action_MoveViewUp; break;
		case ActionId::MoveViewDown:
			functorPtr = &Engine::Action_MoveViewDown; break;
		case ActionId::MoveViewLeft:
			functorPtr = &Engine::Action_MoveViewLeft; break;
		case ActionId::MoveViewRight:
			functorPtr = &Engine::Action_MoveViewRight; break;
		case ActionId::ResetView:
			functorPtr = &Engine::Action_ResetView; break;
		case ActionId::ZoomIn:
			functorPtr = &Engine::Action_ZoomIn; break;
		case ActionId::ZoomOut:
			functorPtr = &Engine::Action_ZoomOut; break;
		case ActionId::ResetZoom:
			functorPtr = &Engine::Action_ResetZoom; break;
		case ActionId::Save:
			functorPtr = &Engine::Action_Save; break;
		case ActionId::Quit:
			functorPtr = &Engine::Action_Quit; break;
		default:
			functorPtr = &Engine::Action_INVALID_ACTION; break;
		}

	case EngineState::Paused:

		switch (t_id) {
		case ActionId::Unpause:
			functorPtr = &Engine::Action_Unpause; break;
		case ActionId::MoveViewUp:
			functorPtr = &Engine::Action_MoveViewUp; break;
		case ActionId::MoveViewDown:
			functorPtr = &Engine::Action_MoveViewDown; break;
		case ActionId::MoveViewLeft:
			functorPtr = &Engine::Action_MoveViewLeft; break;
		case ActionId::MoveViewRight:
			functorPtr = &Engine::Action_MoveViewRight; break;
		case ActionId::ResetView:
			functorPtr = &Engine::Action_ResetView; break;
		case ActionId::ZoomIn:
			functorPtr = &Engine::Action_ZoomIn; break;
		case ActionId::ZoomOut:
			functorPtr = &Engine::Action_ZoomOut; break;
		case ActionId::ResetZoom:
			functorPtr = &Engine::Action_ResetZoom; break;
		case ActionId::Save:
			functorPtr = &Engine::Action_Save; break;
		case ActionId::Quit:
			functorPtr = &Engine::Action_Quit; break;
		default:
			functorPtr = &Engine::Action_INVALID_ACTION; break;
		}
	default:
		functorPtr = &Engine::Action_INVALID_ACTION; break;

	}

	return std::bind(functorPtr, this, std::placeholders::_1);
	// We can do this because the engine owns the event handler.
	// Regularly a class doesnt pass callbacks of its members without dynamic binding
	// else it risks the functor being called with a dangling this pointer.
}

#if defined(_DEBUG) && IS_PRINT_TRIGGERED_ACTIONS_TO_CONSOLE == 2
#include <iostream>

static const std::string ACTION_MESSAGE_IDENTIFER{ "> ACTION\t" };
void Engine::Action_Pause(const EventInfo& t_info) { std::cout << ACTION_MESSAGE_IDENTIFER << getActionStr(ActionId::Pause) << std::endl; }
void Engine::Action_Unpause(const EventInfo& t_info) { std::cout << ACTION_MESSAGE_IDENTIFIER << getActionStr(ActionId::UnPause) << std::endl; }
void Engine::Action_MoveViewLeft(const EventInfo& t_info) { std::cout << ACTION_MESSAGE_IDENTIFER << getActionStr(ActionId::MoveViewLeft) << std::endl; }
void Engine::Action_MoveViewRight(const EventInfo& t_info) { std::cout << ACTION_MESSAGE_IDENTIFER << getActionStr(ActionId::MoveViewRight) << std::endl; }
void Engine::Action_MoveViewUp(const EventInfo& t_info) { std::cout << ACTION_MESSAGE_IDENTIFER << getActionStr(ActionId::MoveViewUp) << std::endl; }
void Engine::Action_MoveViewDown(const EventInfo& t_info) { std::cout << ACTION_MESSAGE_IDENTIFER << getActionStr(ActionId::MoveViewDown) << std::endl; }
void Engine::Action_ResetView(const EventInfo& t_info) { std::cout << ACTION_MESSAGE_IDENTIFER << getActionStr(ActionId::ResetView) << std::endl; }
void Engine::Action_ZoomIn(const EventInfo& t_info) { std::cout << ACTION_MESSAGE_IDENTIFER << getActionStr(ActionId::ZoomIn) << std::endl; }
void Engine::Action_ZoomOut(const EventInfo& t_info) { std::cout << ACTION_MESSAGE_IDENTIFER << getActionStr(ActionId::ZoomOut) << std::endl; }
void Engine::Action_ResetZoom(const EventInfo& t_info) { std::cout << ACTION_MESSAGE_IDENTIFER << getActionStr(ActionId::ResetZoom) << std::endl; }
void Engine::Action_Save(const EventInfo& t_info) { std::cout << ACTION_MESSAGE_IDENTIFER << getActionStr(ActionId::Save) << std::endl; }
void Engine::Action_Quit(const EventInfo& t_info) { std::cout << ACTION_MESSAGE_IDENTIFER << getActionStr(ActionId::Quit) << std::endl; }
void Engine::Action_INVALID_ACTION(const EventInfo& t_info) { std::cout << ACTION_MESSAGE_IDENTIFER << "INVALID_ACTION" << std::endl; }

#else

void Engine::Action_Pause(const EventInfo& t_info) { m_state = EngineState::Paused; }
void Engine::Action_Unpause(const EventInfo& t_info) { m_state = EngineState::Running; }
void Engine::Action_MoveViewLeft(const EventInfo& t_info) {}
void Engine::Action_MoveViewRight(const EventInfo& t_info) {}
void Engine::Action_MoveViewUp(const EventInfo& t_info) {}
void Engine::Action_MoveViewDown(const EventInfo& t_info) {}
void Engine::Action_ResetView(const EventInfo& t_info) {}
void Engine::Action_ZoomIn(const EventInfo& t_info) { m_view.zoom(1.f + m_viewZoom); }
void Engine::Action_ZoomOut(const EventInfo& t_info) { m_view.zoom(1.f - m_viewZoom); }
void Engine::Action_ResetZoom(const EventInfo& t_info) { resetView(); }
void Engine::Action_Save(const EventInfo& t_info) {}
void Engine::Action_Quit(const EventInfo& t_info) {}
void Engine::Action_INVALID_ACTION(const EventInfo& t_info) {}

#endif