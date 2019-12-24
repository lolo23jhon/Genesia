#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <SFML/Window/Event.hpp>

// Stores tehe string names of all the existing keys in the keyboard for file writting, parsing and HUI purposes
// Also serves as a list of all potentially mapable keys (its a subset of the SFML keys)
using KeyNames = std::unordered_map<std::string, sf::Keyboard::Key>;

struct KeyInfo;

// Contains the information for ALL the keys that sould be listened to (whether they're pressed or not; just for internal usage)
using KeyContainer = std::unordered_map<sf::Keyboard::Key, KeyInfo >;

static const std::string S_EMPTY_STR{ "" };

class KeyInfo {
	friend class Keyboard;

	const std::string& m_name; // Reference to the KeyContainer which has all key strings
	bool m_listened; // Should the key be listened to?
	bool m_isPressed; // Is the key currently presed (pressed event and no release event yet)?

	////////////////////////////////////////////////////////////
	KeyInfo(const std::string& t_name, bool t_listened = false, bool t_isPressed = false) :
		m_name{ t_name }, m_listened{ t_listened }, m_isPressed{ t_isPressed }{}
};

class Keyboard {
public:
	////////////////////////////////////////////////////////////
	Keyboard() {
		for (auto& it : s_keyNames) {
			m_keyContainer.try_emplace(it.second, it.first, false, false);
		}
	}


	////////////////////////////////////////////////////////////
	void handleKeyboardInput(const sf::Event& t_e) {
		auto it{ m_keyContainer.find(t_e.key.code) };
		if (it == m_keyContainer.end()) { return; }
		switch (t_e.type) {
		case sf::Event::KeyPressed:
			if (!it->second.m_isPressed) { it->second.m_isPressed = true; }
			break;
		case sf::Event::KeyReleased:
			if (it->second.m_isPressed) { it->second.m_isPressed = false; };
			break;
		}
	}

	////////////////////////////////////////////////////////////
	void listenToKey(const sf::Keyboard::Key& t_key) {
		auto it{ m_keyContainer.find(t_key) };
		if (it == m_keyContainer.end()) { return; }
		it->second.m_listened = true;
	}

	////////////////////////////////////////////////////////////
	void stopListeningToKey(const sf::Keyboard::Key& t_key) {
		auto it{ m_keyContainer.find(t_key) };
		if (it == m_keyContainer.end()) { return; }
		it->second.m_listened = false;
	}

	////////////////////////////////////////////////////////////
	bool isKeyDown(const sf::Keyboard::Key& t_key) {
		auto it{ m_keyContainer.find(t_key) };
		if (it == m_keyContainer.end()) { return false; }
		if (!it->second.m_listened) { return false; }
		return it->second.m_isPressed;
	}

	////////////////////////////////////////////////////////////
	static bool isKeySupported(const sf::Keyboard::Key& t_key) {
		auto it{ std::find_if(s_keyNames.begin(),s_keyNames.end(),
			[&t_key](std::pair<std::string, sf::Keyboard::Key> t_p) {return t_p.second == t_key; }) };
		return (it != s_keyNames.end());
	}

	////////////////////////////////////////////////////////////
	static const std::string& getKeyStr(const sf::Keyboard::Key& t_key) {
		auto it{ std::find_if(s_keyNames.cbegin(),s_keyNames.cend(),
			[&t_key](const std::pair<std::string,sf::Keyboard::Key>& t_p) {return t_p.second == t_key; }) };
		return  (it == s_keyNames.cend() ? S_EMPTY_STR : it->first);
	}

	////////////////////////////////////////////////////////////
	static sf::Keyboard::Key getKeyId(const std::string& t_name) {
		auto it{ s_keyNames.find(t_name) };
		return (it == s_keyNames.cend() ? sf::Keyboard::Unknown : it->second);
	}


private:
	static const KeyNames s_keyNames;
	KeyContainer m_keyContainer;


};

const KeyNames Keyboard::s_keyNames{
	{"A",sf::Keyboard::Key::A },
	{"B",sf::Keyboard::Key::B },
	{"C",sf::Keyboard::Key::C },
	{"D",sf::Keyboard::Key::D },
	{"E",sf::Keyboard::Key::E },
	{"F",sf::Keyboard::Key::F },
	{"G",sf::Keyboard::Key::G },
	{"H",sf::Keyboard::Key::H },
	{"I",sf::Keyboard::Key::I },
	{"J",sf::Keyboard::Key::J },
	{"K",sf::Keyboard::Key::K },
	{"L",sf::Keyboard::Key::L },
	{"M",sf::Keyboard::Key::M },
	{"N",sf::Keyboard::Key::N },
	{"O",sf::Keyboard::Key::O },
	{"P",sf::Keyboard::Key::P },
	{"Q",sf::Keyboard::Key::Q },
	{"R",sf::Keyboard::Key::R },
	{"S",sf::Keyboard::Key::S },
	{"T",sf::Keyboard::Key::T },
	{"U",sf::Keyboard::Key::U },
	{"V",sf::Keyboard::Key::V },
	{"W",sf::Keyboard::Key::W },
	{"X",sf::Keyboard::Key::X },
	{"Y",sf::Keyboard::Key::Y },
	{"Z",sf::Keyboard::Key::Z },

	// Alpha numbers
	{"Num0",sf::Keyboard::Key::Num0 },
	{"Num1",sf::Keyboard::Key::Num1 },
	{"Num2",sf::Keyboard::Key::Num2 },
	{"Num3",sf::Keyboard::Key::Num3 },
	{"Num4",sf::Keyboard::Key::Num4 },
	{"Num5",sf::Keyboard::Key::Num5 },
	{"Num6",sf::Keyboard::Key::Num6 },
	{"Num7",sf::Keyboard::Key::Num7 },
	{"Num8",sf::Keyboard::Key::Num8 },
	{"Num9",sf::Keyboard::Key::Num9 },

	// Numpad numbers
	{"Numpad0",sf::Keyboard::Key::Numpad0 },
	{"Numpad1",sf::Keyboard::Key::Numpad1 },
	{"Numpad2",sf::Keyboard::Key::Numpad2 },
	{"Numpad3",sf::Keyboard::Key::Numpad3 },
	{"Numpad4",sf::Keyboard::Key::Numpad4 },
	{"Numpad5",sf::Keyboard::Key::Numpad5 },
	{"Numpad6",sf::Keyboard::Key::Numpad6 },
	{"Numpad7",sf::Keyboard::Key::Numpad7 },
	{"Numpad8",sf::Keyboard::Key::Numpad8 },
	{"Numpad9",sf::Keyboard::Key::Numpad9 },

	// Others
	{"Space",	sf::Keyboard::Key::Space},
	{"Up",		sf::Keyboard::Key::Up	},
	{"Down",	sf::Keyboard::Key::Down	},
	{"Left",	sf::Keyboard::Key::Left	},
	{"Right",	sf::Keyboard::Key::Right}
};

#endif // !KEYBOARD_H
