#include "BoundKeys.h"

////////////////////////////////////////////////////////////
BoundKeys::BoundKeys() : m_keys{} {}

////////////////////////////////////////////////////////////
bool BoundKeys::hasKey(const sf::Keyboard::Key& t_key)const {
	return m_keys.find(t_key) == m_keys.end();
}

////////////////////////////////////////////////////////////
void BoundKeys::addKey(const sf::Keyboard::Key& t_key) {
	m_keys.emplace(t_key);
}

////////////////////////////////////////////////////////////
void BoundKeys::removeKey(const sf::Keyboard::Key& t_key) {
	m_keys.erase(t_key);
}

////////////////////////////////////////////////////////////
bool BoundKeys::checkMatch(const PressedKeys& t_pressedKeys)const {
	for (auto& k : t_pressedKeys) {
		auto match{m_keys.find(k)};
		if (match != m_keys.end()) { return true; }
	}
	return false;
}