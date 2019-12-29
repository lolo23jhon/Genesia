#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <SFML/Window/Event.hpp>



class KeyInfo;
using KeyNames = std::unordered_map<std::string, sf::Keyboard::Key>;
using KeyContainer = std::unordered_map<sf::Keyboard::Key, KeyInfo >; // Contains the information for ALL the keys that sould be listened to (whether they're pressed or not; just for internal usage)

using KeySet = std::unordered_set<sf::Keyboard::Key>;

class Keyboard {
public:
	Keyboard();
	void handleKeyboardInput(const sf::Event& t_e);
	void listenToKey(const sf::Keyboard::Key& t_key);
	void stopListeningToKey(const sf::Keyboard::Key& t_key);
	bool isKeyDown(const sf::Keyboard::Key& t_key)const;
	bool isKeyBeingReleased(const sf::Keyboard::Key& t_key)const;
	const KeySet& getPressedKeys()const;
	const KeySet& getReleasedKeys()const;
	void reset(); // Must run this ouside the event poll loop

	static bool isKeySupported(const sf::Keyboard::Key& t_keyId);
	static bool isKeySupported(const std::string& t_keyName);
	static const std::string& getKeyStr(const sf::Keyboard::Key& t_key);
	static sf::Keyboard::Key getKeyId(const std::string& t_name);

private:
	static const KeyNames s_keyNames;
	KeySet m_listenedKeys;
	KeySet m_pressedKeys;  // Keys currently pressed
	KeySet m_releasedKeys; // Keys currently being released

};

#endif // !KEYBOARD_H
