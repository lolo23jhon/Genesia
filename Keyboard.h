#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <SFML/Window/Event.hpp>


using KeyNames = std::unordered_map<std::string, sf::Keyboard::Key>;// Stores tehe string names of all the existing keys in the keyboard for file writting, parsing and HUI purposes
// Also serves as a list of all potentially mapable keys (its a subset of the SFML keys)

struct KeyInfo;
																			
using KeyContainer = std::unordered_map<sf::Keyboard::Key, KeyInfo >; // Contains the information for ALL the keys that sould be listened to (whether they're pressed or not; just for internal usage)

using PressedKeys = std::unordered_set<sf::Keyboard::Key>;

class KeyInfo {
	friend class Keyboard;

	const std::string& m_name;		// Reference to the KeyContainer which has all key strings
	bool m_listened;				// Should the key be listened to?
	bool m_isPressed;				// Is the key currently presed (pressed event and no release event yet)?

	KeyInfo(const std::string& t_name, bool t_listened = false, bool t_isPressed = false);
};

class Keyboard {
public:
	Keyboard();
	void handleKeyboardInput(const sf::Event& t_e);
	void listenToKey(const sf::Keyboard::Key& t_key);
	void stopListeningToKey(const sf::Keyboard::Key& t_key);
	bool isKeyDown(const sf::Keyboard::Key& t_key);
	const PressedKeys& getPressedKeys();

	static bool isKeySupported(const sf::Keyboard::Key& t_key);
	static const std::string& getKeyStr(const sf::Keyboard::Key& t_key);
	static sf::Keyboard::Key getKeyId(const std::string& t_name);

private:
	static const KeyNames s_keyNames;
	KeyContainer m_keyContainer;
	PressedKeys m_pressedKeys;


};


#endif // !KEYBOARD_H
