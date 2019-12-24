#ifndef COLOR_MAP_H
#define COLOR_MAP_H

#include <map>
#include <SFML/Graphics/Color.hpp>


class ColorMap {

	sf::Color m_defaultColor;
	std::map <double, sf::Color> m_colorLimits; // The color is applied from that height downwards

public:
	ColorMap(const std::map<double, sf::Color>& t_colorRanges = {}, const sf::Color& t_defCol = sf::Color::Black);
	ColorMap& setNextColor(double t_heightLim, const sf::Color& t_color);
	const sf::Color& getColor(double t_height) const;
};

#endif