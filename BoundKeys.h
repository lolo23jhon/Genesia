#ifndef BOUND_KEYS_H
#define BOUND_KEYS_H

#include <string>
#include <unordered_set>
#include <SFML/Window/Keyboard.hpp>
#include "Keyboard.h"

using KeySet = std::unordered_set <sf::Keyboard::Key>; // Non owning pointsrs to the supported key list in keyboard class

class BoundKeys {
	KeySet m_keys; // Contains all the possible keys that can trigger that action
public:
	BoundKeys();
	bool hasKey(const sf::Keyboard::Key& t_key)const;
	void addKey(const sf::Keyboard::Key& t_key); // Adds another possible key that can execute this action
	void removeKey(const sf::Keyboard::Key& t_key); // Removes the key
	bool checkMatch(const KeySet& t_pressedKeys)const;
};

#endif // !BOUND_KEYS_H