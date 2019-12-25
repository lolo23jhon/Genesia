#include "Engine.h"


// ------------------------------------------------------------------------------------------------------------
//                                                 ACTIONS
// ------------------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////
const ActionNames Engine::s_actionNames{
	{"Pause",			Engine::ActionId::Pause},
	{"MoveViewUp",		Engine::ActionId::MoveViewUp},
	{"MoveViewDown",	Engine::ActionId::MoveViewDown},
	{"MoveViewLeft",	Engine::ActionId::MoveViewLeft},
	{"MoveViewRight",	Engine::ActionId::MoveViewRight},
	{"ResetView",		Engine::ActionId::ResetView},
	{"ZoomIn",			Engine::ActionId::ZoomIn},
	{"ZoomOut",			Engine::ActionId::ZoomOut},
	{"ResetZoom",		Engine::ActionId::ResetZoom},
	{"Save",			Engine::ActionId::Save},
	{"Quit",			Engine::ActionId::Quit}
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


//void Engine::Action_Pause(const EventInfo& t_info);
//void Engine::Action_MoveViewLeft(const EventInfo& t_info);
//void Engine::Action_MoveViewRight(const EventInfo& t_info);
//void Engine::Action_MoveViewUp(const EventInfo& t_info);
//void Engine::Action_MoveViewDown(const EventInfo& t_info);
//void Engine::Action_ResetView(const EventInfo& t_info);
//void Engine::Action_ZoomIn(const EventInfo& t_info);
//void Engine::Action_ZoomOut(const EventInfo& t_info);
//void Engine::Action_ResetZoom(const EventInfo& t_info);
//void Engine::Action_Save(const EventInfo& t_info);
//void Engine::Action_Quit(const EventInfo& t_info);

#ifdef _DEBUG
#include <iostream>
static const std::string ACTION{ "Action_" };
void Engine::Action_Pause			(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::Pause)		 << std::endl; }
void Engine::Action_MoveViewLeft	(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::MoveViewLeft)  << std::endl; }
void Engine::Action_MoveViewRight	(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::MoveViewRight) << std::endl; }
void Engine::Action_MoveViewUp		(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::MoveViewUp)	 << std::endl; }
void Engine::Action_MoveViewDown	(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::MoveViewDown)  << std::endl; }
void Engine::Action_ResetView		(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::ResetView)	 << std::endl; }
void Engine::Action_ZoomIn			(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::ZoomIn)		 << std::endl; }
void Engine::Action_ZoomOut			(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::ZoomOut)		 << std::endl; }
void Engine::Action_ResetZoom		(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::ResetZoom)	 << std::endl; }
void Engine::Action_Save			(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::Save)			 << std::endl; }
void Engine::Action_Quit			(const EventInfo& t_info) { std::cout << ACTION << getActionStr(ActionId::Quit)			 << std::endl; }

#else

#endif


////////////////////////////////////////////////////////////
Engine::Engine(const sf::Vector2u& t_windowSize, const std::string& t_windowName) :
	m_window{ sf::VideoMode(t_windowSize.x,t_windowSize.y),t_windowName }
{
}


////////////////////////////////////////////////////////////
void Engine::init() {}

////////////////////////////////////////////////////////////
void Engine::run() {

	// Window loop
	while (m_window.isOpen()) {

		// Restar the clock and capture elapsed time
		m_elapsed = m_clock.restart();

		sf::Event e;




	}

	m_window.close();
}


////////////////////////////////////////////////////////////
const sf::RenderWindow& Engine::getWindow()const {
	return m_window;
}





////////////////////////////////////////////////////////////
void handleInput(const sf::Keyboard::Key& t_key, const float& s_dt, bool t_pressed) {


}



////////////////////////////////////////////////////////////
void Engine::render() {

	// Clear window

}


////////////////////////////////////////////////////////////
void Engine::resetView() {

}

////////////////////////////////////////////////////////////
void Engine::processEvents() {
	sf::Event e;

	while (m_window.pollEvent(e)) {
		switch (e.type) {
		case sf::Event::KeyPressed:


			break;



		case sf::Event::KeyReleased:

			break;

		case sf::Event::Closed:
			m_window.close();
			break;
		}
	}
}
