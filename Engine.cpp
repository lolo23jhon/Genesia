#include "PreprocessorDirectves.h"
#include "Utilities.h"
#include "Engine.h"


// ------------------------------------------------------------------------------------------------------------
//                                                 ACTIONS
// ------------------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////
const ActionNames Engine::s_actionNames{
	{"Pause",			ActionId::Pause},
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

static const std::string S_EMPTY_STR{ "" };

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
bool Engine::executeAction(const ActionId& t_id, const EventInfo& t_info) {
	return m_eventHandler.executeAction(t_id, t_info);
}

////////////////////////////////////////////////////////////
sf::Time Engine::getElapsed()const { return m_elapsed; }

#if defined(_DEBUG) && SHOW_ACTIONS_IN_CONSOLE == 1
#include <iostream>

static const std::string ACTION{ "Action_" };
void Engine::Action_Pause(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::Pause) << std::endl; }
void Engine::Action_MoveViewLeft(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::MoveViewLeft) << std::endl; }
void Engine::Action_MoveViewRight(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::MoveViewRight) << std::endl; }
void Engine::Action_MoveViewUp(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::MoveViewUp) << std::endl; }
void Engine::Action_MoveViewDown(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::MoveViewDown) << std::endl; }
void Engine::Action_ResetView(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::ResetView) << std::endl; }
void Engine::Action_ZoomIn(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::ZoomIn) << std::endl; }
void Engine::Action_ZoomOut(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::ZoomOut) << std::endl; }
void Engine::Action_ResetZoom(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::ResetZoom) << std::endl; }
void Engine::Action_Save(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::Save) << std::endl; }
void Engine::Action_Quit(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::Quit) << std::endl; }

#else
void Engine::Action_Pause(const EventInfo& t_info) {}
void Engine::Action_MoveViewLeft(const EventInfo& t_info) {}
void Engine::Action_MoveViewRight(const EventInfo& t_info) {}
void Engine::Action_MoveViewUp(const EventInfo& t_info) {}
void Engine::Action_MoveViewDown(const EventInfo& t_info) {}
void Engine::Action_ResetView(const EventInfo& t_info) {}
void Engine::Action_ZoomIn(const EventInfo& t_info) {}
void Engine::Action_ZoomOut(const EventInfo& t_info) {}
void Engine::Action_ResetZoom(const EventInfo& t_info) {}
void Engine::Action_Save(const EventInfo& t_info) {}
void Engine::Action_Quit(const EventInfo& t_info) {}


#endif
void Engine::Action_INVALID_ACTION(const EventInfo& t_info) { std::cout << "Action_" << "INVALID_ACTION" << std::endl; }


////////////////////////////////////////////////////////////
Engine::Engine(const sf::Vector2u& t_windowSize, const std::string& t_windowName) :
	m_window{ sf::VideoMode(t_windowSize.x,t_windowSize.y),t_windowName },
	m_keyboard{ Keyboard() },
	m_eventHandler{ EventHandler() }
{
	for (const auto& p : s_actionNames) {
		const auto& actionId{ p.second };
		m_eventHandler.addCallback(actionId, getActionCallback(actionId));
	}
	if (!parseBindings("keybindings.txt")) {
		std::cout << "Press Enter to exit.\n";
		std::cin.get();
		std::exit(1);
	}
}

////////////////////////////////////////////////////////////
ActionCallback Engine::getActionCallback(const ActionId& t_id) {
	ActionFunctor functorPtr;
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

	return std::bind(functorPtr, this, std::placeholders::_1);
	// We can do this because the engine owns the event handler.
	// Regularly a class doesnt pass callbacks of its members without dynamic binding
	// else it risks the functor being called with a dangling this pointer.
}











////////////////////////////////////////////////////////////
void Engine::init() {}

////////////////////////////////////////////////////////////
void Engine::run() {

	// Window loop
	while (m_window.isOpen()) {

		// Restar the clock and capture elapsed time
		m_elapsed = m_clock.restart();

		//                   ##%
		//                 #%%#   
		//				  %#
		//               | |
		//              _| |\|\|\|\
		// sf::Event -> |   __     | -> EventInfo
		//				|__|  |____|

		sf::Event e;
		while (m_window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) { m_window.close(); }
			m_keyboard.handleKeyboardInput(e);
			auto pressedKeys{ m_keyboard.getPressedKeys() };
			EventInfo eventInfo(m_state, pressedKeys, m_elapsed.asSeconds());

			m_eventHandler.handleEvent(eventInfo);

		}
	}
	
}


////////////////////////////////////////////////////////////
const sf::RenderWindow& Engine::getWindow()const {
	return m_window;
}


////////////////////////////////////////////////////////////
void Engine::render() {

	// Clear window

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

			if (!m_eventHandler.addKey(actionId, keyId)) {
				std::cerr << "! WARNING: Failed to add key \"" << key << "\" to action  \"" << action << '\"' << std::endl;
			}

			m_keyboard.listenToKey(keyId); // Tell the keyboard to listen to the key
		}
		else { continue; }// The token wasn't the line identifier; skip it
	}

	return true;
}