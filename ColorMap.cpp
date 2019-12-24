#include "ColorMap.h"




////////////////////////////////////////////////////////////
ColorMap::ColorMap(const std::map<double, sf::Color>& t_colorLimits, const sf::Color& t_defCol) :
	m_colorLimits{ t_colorLimits }, m_defaultColor{ t_defCol }{}


////////////////////////////////////////////////////////////
ColorMap& ColorMap::setNextColor(double t_heightLim, const sf::Color& t_color) {
	m_colorLimits.emplace(t_heightLim, t_color);
	return *this;
}


////////////////////////////////////////////////////////////
const sf::Color& ColorMap::getColor(double t_height) const {
	for (auto it{ m_colorLimits.cbegin() }; it != m_colorLimits.cend(); it++) {
		if (t_height <= it->first) {
			return it->second;
		}
	}
	return m_defaultColor;
}
