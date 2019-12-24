#include "Engine.h"
///////////////////////////////////////////////////////////



bool Binding::isActive() {
	for (auto it{ m_keys.begin() }; it != m_keys.end(); it++) {
		if (it->second) { return true; }
	}
	return false;
}

////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
const ActionNames Engine::s_actionNames{
	{"Pause",			Action::Pause},
	{"MoveViewUp",		Action::MoveViewUp},
	{"MoveViewDown",	Action::MoveViewDown},
	{"MoveViewLeft",	Action::MoveViewLeft},
	{"MoveViewRight",	Action::MoveViewRight},
	{"ResetView",		Action::ResetView},
	{"ZoomIn",			Action::ZoomIn},
	{"ZoomOut",			Action::ZoomOut},
	{"ResetZoom",		Action::ResetZoom},
	{"Save",			Action::Save},
	{"Quit",			Action::Quit}
};

////////////////////////////////////////////////////////////
Action Engine::getActionId(const std::string& t_actionName) {
	auto it{ s_actionNames.find(t_actionName) };
	return (it != s_actionNames.end() ? it->second : Action::INVALID_ACTION);
}

////////////////////////////////////////////////////////////
const std::string& Engine::getActionName(const Action& t_actionId) {
	auto it{ std::find_if(s_actionNames.begin(),s_actionNames.end(),
		[&t_actionId](std::pair<std::string, Action>& t_pair) {return t_pair.second == t_actionId; }) };
	return (it != s_actionNames.end() ? it->first : INVALID_STR);
}


////////////////////////////////////////////////////////////
Key Engine::getKeyId(const std::string& t_keyName) {
	auto it{ s_keyNames.find(t_keyName) };
	return (it != s_keyNames.end() ? it->second : Key::INVALID_KEY);
}

////////////////////////////////////////////////////////////
const std::string& Engine::getKeyName(const Key& t_keyId) {
	auto it{ std::find_if(s_keyNames.begin(),s_keyNames.end(),
		[&t_keyId](std::pair<std::string,Key>& t_pair) {return t_pair.second == t_keyId; }) };
	return (it != s_keyNames.end() ? it->first : INVALID_STR);
}


////////////////////////////////////////////////////////////
Engine::Engine(const sf::Vector2u& t_windowSize, const std::string& t_windowName) :
	m_window{ sf::VideoMode(t_windowSize.x,t_windowSize.y),t_windowName } 
{
}


////////////////////////////////////////////////////////////
void Engine::init() {




}

////////////////////////////////////////////////////////////
void Engine::run() {

	// Window loop
	while ( m_window.isOpen() ) {
		
		// Restar the clock and capture elapsed time
		m_elapsed = m_clock.restart();

		sf::Event e;

		while (m_state == State::RUNNING) {



		}



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
